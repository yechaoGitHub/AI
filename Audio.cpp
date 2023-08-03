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

void Audio::Initialize()
{
    connect(this, &Audio::startListen, this, &Audio::StartListen);
    connect(this, &Audio::startPlay, this, &Audio::StartPlay);
    connect(this, &Audio::endListen, this, &Audio::EndListen);
    connect(this, &Audio::endPlay, this, &Audio::EndPlay);
}

void Audio::StartListen()
{
    _ioInput = _audioInput->start();
    _inTimer = startTimer(10, Qt::PreciseTimer);
}

void Audio::StartPlay()
{
    _ioOutput = _audioOutput->start();
}

void Audio::EndListen()
{
    killTimer(_inTimer);

    _audioInput->stop();
    _ioInput->close();
    delete _ioInput;
    _ioInput = nullptr;
}

void Audio::EndPlay()
{
    _audioOutput->stop();
    _ioOutput->close();
    delete _ioOutput;
    _ioOutput = nullptr;
}

void Audio::StartListenAsync()
{
    emit startListen();
}

void Audio::StartPlayAsync()
{
    emit startPlay();
}

void Audio::EndListenAsync()
{
    emit endListen();
}

void Audio::EndPlayAsync()
{
    emit endListen();
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
        if (_inLen < 1280)
        {
            auto data = _ioInput->read(1280 - _inLen);
            if (data.size() == 1280)
            {
                _inBufferData = std::move(data);
                _inLen = 1280;
            }
            else
            {
                _inBufferData.push_back(data);
                _inLen += data.size();
            }
        }

        auto cur = std::chrono::steady_clock::now();
        auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(cur - _inlastTick);
        if (interval.count() > 40)
        {
            if (_inLen == 1280 && AvgVolume(_inBufferData) > 1000)
            {
                emit audioInput(std::move(_inBufferData));

                _inBufferData.resize(1280);
                _inLen = 0;
                _inlastTick = cur;
            }
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