#include "AudioOutput.h"

AudioOutput::AudioOutput()
{

}

AudioOutput::~AudioOutput()
{
    Uninitialize();
}

void AudioOutput::Initialize()
{
    _devInfo = QAudioDeviceInfo::defaultOutputDevice();

    connect(this, &AudioOutput::start_speaker, this, &AudioOutput::StartSpeakerInternal);
    connect(this, &AudioOutput::end_speaker, this, &AudioOutput::EndSpeakerInternal);
    connect(this, &AudioOutput::write_output_data, this, &AudioOutput::WriteOutputDataInternal);
    this->moveToThread(&_workThread);
}

void AudioOutput::Uninitialize()
{
    if (_workThread.isRunning())
    {
        EndSpeaker();
    }
}

void AudioOutput::StartSpeaker()
{
    if (_workThread.isRunning())
    {
        return;
    }

    _workThread.start();
    emit start_speaker();
}

void AudioOutput::StartSpeaker(const QAudioDeviceInfo& info)
{
    if (_workThread.isRunning())
    {
        return;
    }

    _devInfo = info;
    _workThread.start();
    emit start_speaker();
}

void AudioOutput::EndSpeaker()
{
    if (!_workThread.isRunning())
    {
        return;
    }

    emit end_speaker();
    if (!_workThread.wait(1000))
    {
        _workThread.quit();
    }
}

bool AudioOutput::IsRunning()
{
    return _workThread.isRunning();
}

void AudioOutput::WriteOutputData(QByteArray data)
{
    emit write_output_data(std::move(data));
}

void AudioOutput::timerEvent(QTimerEvent* event)
{
    if (_audioData.size() < 1280)
    {
        return;
    }

    int writePos = 0;
    while (writePos < _audioData.size())
    {
        auto free = _audioOutput->bytesFree();
        auto size = _audioOutput->periodSize();
        if (free >= size)
        {
            auto remain = _audioData.size() - writePos;
            auto writeSize = std::min(free, remain);
            _ioOutput->write(_audioData.data() + writePos, writeSize);
            writePos += writeSize;
        }
        else
        {
            std::this_thread::yield();
        }
    }

    _audioData.clear();
}

void AudioOutput::StartSpeakerInternal()
{
    QAudioFormat auidoFormat;
    auidoFormat.setSampleRate(16000);
    auidoFormat.setChannelCount(1);
    auidoFormat.setSampleSize(16);
    auidoFormat.setCodec("audio/pcm");
    auidoFormat.setByteOrder(QAudioFormat::LittleEndian);
    auidoFormat.setSampleType(QAudioFormat::SignedInt);

    _audioOutput = new QAudioOutput{ _devInfo, auidoFormat, this };

    _ioOutput = _audioOutput->start();
    _timer = startTimer(1, Qt::PreciseTimer);

    if (!_ioOutput)
    {
        EndSpeakerInternal();
    }
}

void AudioOutput::EndSpeakerInternal()
{

    killTimer(_timer);
    if (_audioOutput)
    {
        _audioOutput->stop();
        delete _audioOutput;
        _audioOutput = nullptr;
    }

    _ioOutput = nullptr;
    _workThread.quit();
}

void AudioOutput::WriteOutputDataInternal(QByteArray data)
{
    _audioData.push_back(std::move(data));
}
