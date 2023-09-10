#include "AudioInput.h"
#include <thread>
#include <QTimerEvent>
#include <chrono>
#include <QDebug>

AudioInput::AudioInput()
{
}

AudioInput::~AudioInput()
{
    Uninitialize();
}

void AudioInput::Initialize()
{
    _devInfo = QAudioDeviceInfo::defaultInputDevice();

    connect(this, &AudioInput::start_mic, this, &AudioInput::StartMicInternal);
    connect(this, &AudioInput::end_mic, this, &AudioInput::EndMicInternal);
    this->moveToThread(&_workThread);
#ifdef MONITOR_MIC
    _audioOutput.Initialize();
#endif
}

void AudioInput::Uninitialize()
{
    if (_workThread.isRunning())
    {
        EndMic();
    }
}

void AudioInput::StartMic()
{
    if (_workThread.isRunning())
    {
        return;
    }

    _workThread.start();
    emit start_mic();
}

void AudioInput::StartMic(const QAudioDeviceInfo& info)
{
    if (_workThread.isRunning())
    {
        return;
    }

    _devInfo = info;
    _workThread.start();
    emit start_mic();
}

void AudioInput::EndMic()
{
    emit end_mic();
    if (!_workThread.wait(1000))
    {
        _workThread.quit();
    }
    emit soundPlay(false);
}

void AudioInput::timerEvent(QTimerEvent* event)
{
    auto id = event->timerId();
    if (id == _inTimer)
    {
        ReadAudioData(_ioInput, _inLen, _inBufferData, _inlastTick, &AudioInput::audioInput);
    }
}

void AudioInput::StartMicInternal()
{
    QAudioFormat auidoFormat;
    auidoFormat.setSampleRate(16000);
    auidoFormat.setChannelCount(1);
    auidoFormat.setSampleSize(16);
    auidoFormat.setCodec("audio/pcm");
    auidoFormat.setByteOrder(QAudioFormat::LittleEndian);
    auidoFormat.setSampleType(QAudioFormat::UnSignedInt);

    _audioInput = new QAudioInput(_devInfo, auidoFormat, this);
    _audioInput->setBufferSize(1280);

    _ioInput = _audioInput->start();
    _inTimer = startTimer(1, Qt::PreciseTimer);

#ifdef MONITOR_MIC
    _audioOutput.StartSpeaker();
#endif
}

void AudioInput::EndMicInternal()
{
    killTimer(_inTimer);

    _audioInput->stop();
    delete _audioInput;
    _audioInput = nullptr;
    _ioInput = nullptr;

    _workThread.quit();

#ifdef MONITOR_MIC
    _audioOutput.EndSpeaker();
#endif
}

void AudioInput::ReadAudioData(QIODevice* dev, int& readLen, QByteArray& bufferData, std::chrono::steady_clock::time_point& timePoint, void(AudioInput::*sginal)(QByteArray))
{
    if (readLen < 1280)
    {
        auto data = dev->read(1280 - readLen);
        if (data.size() == 0)
        {
            return;
        }

        if (data.size() == 1280)
        {
            bufferData = std::move(data);
            readLen = 1280;
        }
        else
        {
            bufferData.push_back(data);
            readLen += data.size();
        }
    }

    auto cur = std::chrono::steady_clock::now();
    auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(cur - timePoint);
    if (interval.count() > 40)
    {
        if (readLen == 1280)
        {
            if (AvgDb(bufferData) > -15)
            {
#ifdef MONITOR_MIC
                _audioOutput.WriteOutputData(bufferData);
#endif
                emit (this->*sginal)(std::move(bufferData));

                emit soundPlay(true);
            }
            else
            {
                emit soundPlay(false);
                bufferData.clear();
            }

            readLen = 0;
            timePoint = cur;
        }
    }
}

int32_t AudioInput::AvgDb(const QByteArray& data)
{
    uint64_t total{};
    auto count = data.size() / 2;
    auto sample = reinterpret_cast<const uint16_t*>(data.data());
    for (auto i = 0; i < count; i ++)
    {
        total += sample[i];
    }

    uint64_t avg = total / count;
    auto db = 20 * log10((double)avg / (double)UINT16_MAX);
    return db;
}
