#pragma once
#include <QAudioOutput>
#include <QObject>
#include <QThread>
#include <QTimerEvent>
#include <list>

class AudioOutput : public QObject
{
    Q_OBJECT
public:
    AudioOutput();
    ~AudioOutput();

    void Initialize();
    void Uninitialize();

    void StartSpeaker();
    void StartSpeaker(const QAudioDeviceInfo& info);
    void EndSpeaker();

    void WriteOutputData(QByteArray data);

protected:
    void timerEvent(QTimerEvent* event);

private:
Q_SIGNALS:
    void start_speaker();
    void end_speaker();
    void write_output_data(QByteArray data);

private:
    void StartSpeakerInternal();
    void EndSpeakerInternal();
    void WriteOutputDataInternal(QByteArray data);

    QAudioDeviceInfo        _devInfo;
    QAudioOutput*           _audioOutput = nullptr;
    QIODevice*              _ioOutput = nullptr;
    QThread                 _workThread;
    int                     _timer = 0;
    std::list<QByteArray>   _audioQueue;
};
