#pragma once

//#define MONITOR_MIC

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
    void StartMic(const QAudioDeviceInfo& info);
    void EndMic();
    bool IsRunning();

Q_SIGNALS:
    void audioInput(QByteArray data);
    void soundPlay(bool play);
    void volumeLevel(int level);

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

    int32_t AvgDb(const QByteArray& data);

    QAudioDeviceInfo                        _devInfo;
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

