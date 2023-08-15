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
    _outInfo = QAudioDeviceInfo::defaultOutputDevice();

    connect(this, &AudioOutput::start_speaker, this, &AudioOutput::StartSpeakerInternal);
    connect(this, &AudioOutput::end_speaker, this, &AudioOutput::EndSpeakerInternal);
    connect(this, &AudioOutput::write_output_data, this, &AudioOutput::WriteOutputDataInternal);
    this->moveToThread(&_workThread);
}

void AudioOutput::Initialize(const QAudioDeviceInfo& info)
{
    _outInfo = info;

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
    _workThread.start();
    emit start_speaker();
}

void AudioOutput::EndSpeaker()
{
    emit end_speaker();
    if (!_workThread.wait(1000))
    {
        _workThread.quit();
    }
}

void AudioOutput::WriteOutputData(QByteArray data)
{
    emit write_output_data(std::move(data));
}

void AudioOutput::timerEvent(QTimerEvent* event)
{
    for (auto& data : _audioQueue)
    {
        int writePos = 0;
        while (writePos < data.size())
        {
            auto free = _audioOutput->bytesFree();
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

    _audioQueue.clear();
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

    _audioOutput = new QAudioOutput{ _outInfo, auidoFormat, this };
    _audioOutput->setBufferSize(1280);

    _ioOutput = _audioOutput->start();
    _timer = startTimer(1, Qt::PreciseTimer);
}

void AudioOutput::EndSpeakerInternal()
{
    killTimer(_timer);
    _audioOutput->stop();
    delete _audioOutput;
    _audioOutput = nullptr;
    _ioOutput = nullptr;
    _workThread.quit();
}

void AudioOutput::WriteOutputDataInternal(QByteArray data)
{
    _audioQueue.push_back(std::move(data));
}
