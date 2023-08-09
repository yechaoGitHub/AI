#pragma once

#include <QAudioInput>
#include <QObject>
#include <QThread>

#ifdef MONITOR_MIC
#include <AudioOutput.h>
#endif

class AudioInput : public QObject
{
    Q_OBJECT
public:
    AudioInput();
    ~AudioInput();

    void Initialize();
    void Uninitialize();

    void StartMic();
    void EndMic();

Q_SIGNALS:
    void audioInput(QByteArray data);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
Q_SIGNALS:
    void start_mic();
    void end_mic();

private:
    void StartMicInternal();
    void EndMicInternal();

    void ReadAudioData(QIODevice* dev, int& readLen, QByteArray& bufferData, std::chrono::steady_clock::time_point& timePoint, void(AudioInput::*sginal)(QByteArray));

    uint64_t AvgVolume(const QByteArray& data);

    QAudioInput*                            _audioInput = nullptr;
    QThread                                 _workThread;
    QIODevice*                              _ioInput = nullptr;
    int                                     _inTimer = 0;
    QByteArray                              _inBufferData;
    int                                     _inLen = 0;
    std::chrono::steady_clock::time_point   _inlastTick;

#ifdef MONITOR_MIC
    AudioOutput                             _audioOutput;
#endif


};

