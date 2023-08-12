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
    QAudioFormat auidoFormat;
    auidoFormat.setSampleRate(16000);
    auidoFormat.setChannelCount(1);
    auidoFormat.setSampleSize(16);
    auidoFormat.setCodec("audio/pcm");
    auidoFormat.setByteOrder(QAudioFormat::LittleEndian);
    auidoFormat.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    _audioInput = new QAudioInput(info, auidoFormat, this);
    _audioInput->setBufferSize(1280);

    connect(this, &AudioInput::start_mic, this, &AudioInput::StartMicInternal);
    connect(this, &AudioInput::end_mic, this, &AudioInput::EndMicInternal);
    this->moveToThread(&_workThread);
#ifdef MONITOR_MIC
    _audioOutput.Initialize();
#endif
}

void AudioInput::Initialize(const QAudioDeviceInfo& info)
{
    QAudioFormat auidoFormat;
    auidoFormat.setSampleRate(16000);
    auidoFormat.setChannelCount(1);
    auidoFormat.setSampleSize(16);
    auidoFormat.setCodec("audio/pcm");
    auidoFormat.setByteOrder(QAudioFormat::LittleEndian);
    auidoFormat.setSampleType(QAudioFormat::UnSignedInt);

    _audioInput = new QAudioInput(info, auidoFormat, this);
    _audioInput->setBufferSize(1280);

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

    if (_audioInput)
    {
        delete _audioInput;
        _audioInput = nullptr;
        _ioInput = nullptr;
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

void AudioInput::EndMic()
{
    emit end_mic();
    if (!_workThread.wait(1000))
    {
        _workThread.quit();
    }
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
    //_ioInput->close();
    //delete _ioInput;
    _ioInput = nullptr;
    _workThread.quit();

#ifdef MONITOR_MIC
    _audioOutput.EndSpeaker();
#endif
}

static int komijbox_sound_dB(const uint8_t* pcm, int len);

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
            //if (AvgVolume(bufferData) > 50)
            {
#ifdef MONITOR_MIC
                _audioOutput.WriteOutputData(bufferData);
#endif
                emit (this->*sginal)(std::move(bufferData));
            }
            //else
            //{
            //    bufferData.clear();
           // }

            readLen = 0;
            timePoint = cur;
        }
    }
}

uint64_t AudioInput::AvgVolume(const QByteArray& data)
{
    uint64_t total{};
    auto count = data.size() / 2;
    auto sample = reinterpret_cast<const uint16_t*>(data.data());
    for (auto i = 0; i < count; i ++)
    {
        total += sample[i];
    }

    uint64_t avg = total / count;
    auto dB = (int)(20 * log10(avg));
    qDebug() << dB << "\n";
    return dB;
}

static int komijbox_sound_dB(const uint8_t* pcm, int len)
{
    int sum = 0;
    int dB = 0;
    short tmp = 0;
    short* pcmaddr = (short*)pcm;

    for (int i = 0; i < len; i += 2) {
        memcpy(&tmp, pcmaddr + i, sizeof(short)); // 获取2字节PCM数据
        sum += abs(tmp); // 绝对值求和累加
    }

    sum = sum / (len / 2); // 求PCM数据的平均值，2个字节表示一个16Bit PCM采样数据
    if (sum) {
        dB = (int)(20 * log10(sum));
    }

    return dB;
}
