#include "AudioOutput.h"

AudioOutput::AudioOutput()
{

}

AudioOutput::~AudioOutput()
{
}

void AudioOutput::Initialize()
{
    QAudioFormat auidoFormat;
    auidoFormat.setSampleRate(16000);
    auidoFormat.setChannelCount(1);
    auidoFormat.setSampleSize(16);
    auidoFormat.setCodec("audio/pcm");
    auidoFormat.setByteOrder(QAudioFormat::LittleEndian);
    auidoFormat.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo outInfo = QAudioDeviceInfo::defaultOutputDevice();
    _audioOutput = new QAudioOutput{ outInfo, auidoFormat, this };
    _audioOutput->setBufferSize(1280);

    connect(this, &AudioOutput::start_speaker, this, &AudioOutput::StartSpeakerInternal);
    connect(this, &AudioOutput::end_speaker, this, &AudioOutput::EndSpeakerInternal);
    connect(this, &AudioOutput::write_output_data, this, &AudioOutput::WriteOutputDataInternal);
    this->moveToThread(&_workThread);
}

void AudioOutput::StartSpeaker()
{
    _workThread.start();
    emit start_speaker();
}

void AudioOutput::EndSpeaker()
{
    emit end_speaker();
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
    _ioOutput = _audioOutput->start();
    _timer = startTimer(1, Qt::PreciseTimer);
}

void AudioOutput::EndSpeakerInternal()
{
    killTimer(_timer);
    _audioOutput->stop();
    _ioOutput->close();
    delete _ioOutput;
    _ioOutput = nullptr;
    _workThread.quit();
}

void AudioOutput::WriteOutputDataInternal(QByteArray data)
{
    _audioQueue.push_back(std::move(data));
}
