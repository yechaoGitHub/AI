#include "Audio.h"
#include <thread>
#include <QTimerEvent>
#include <chrono>

Audio::Audio() :
    _audioInput{ nullptr },
    _audioOutput{ nullptr },
    _ioInput{ nullptr },
    _ioOutput{ nullptr }
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

    QAudioDeviceInfo outInfo = QAudioDeviceInfo::defaultOutputDevice();
    _audioOutput = new QAudioOutput{ outInfo, auidoFormat, this };
    _audioOutput->setBufferSize(1280);
}

Audio::~Audio()
{
}

void Audio::StartMic()
{
    _ioInput = _audioInput->start();
    _inTimer = startTimer(10, Qt::PreciseTimer);
}

void Audio::StartSpeaker()
{
    _ioOutput = _audioOutput->start();
}

void Audio::EndMic()
{
    killTimer(_inTimer);

    _audioInput->stop();
    _ioInput->close();
    delete _ioInput;
    _ioInput = nullptr;
}

void Audio::EndSpeaker()
{
    _audioOutput->stop();
    _ioOutput->close();
    delete _ioOutput;
    _ioOutput = nullptr;
}

void Audio::WriteOutputData(const QByteArray& data)
{
    int per_size = _audioOutput->periodSize();
    int writePos = 0;

    while (writePos < data.size())
    {
        auto free =_audioOutput->bytesFree();
        if (free > 0)
        {
            auto remain = data.size() - writePos;
            auto writeSize = std::min(free, remain);
            _ioOutput->write(data.data() + writePos, writeSize);
            writePos += writeSize;
        }
        else 
        {
            std::this_thread::yield();
        }
    }
}

void Audio::timerEvent(QTimerEvent* event)
{
    auto id = event->timerId();
    if (id == _inTimer) 
    {
        ReadAudioData(_ioInput, _inLen, _inBufferData, _inlastTick, &Audio::audioInput);
    }
}

void Audio::ReadAudioData(QIODevice* dev, int& readLen, QByteArray& bufferData, std::chrono::steady_clock::time_point& timePoint, void(Audio::*sginal)(QByteArray))
{
    if (readLen < 1280)
    {
        auto data = dev->read(1280 - readLen);
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
            emit (this->*sginal)(std::move(bufferData));

            bufferData.resize(1280);
            readLen = 0;
            timePoint = cur;
        }
    }
}

uint64_t Audio::AvgVolume(const QByteArray& data)
{
    uint64_t total{};
    auto count = data.size() / 2;
    auto sample = reinterpret_cast<const uint16_t*>(data.data());
    for (auto i = 0; i < count; i ++)
    {
        total += sample[i];
    }

    return total / count;
}
