#pragma once
#include <QAudioInput>
#include <QAudioOutput>
#include <QObject>
#include <QThread>

class Audio : public QObject
{
    Q_OBJECT
public:
    Audio();
    ~Audio();

    void Initialize();

    void StartListen();
    void StartPlay();
    void EndListen();
    void EndPlay();

    void StartListenAsync();
    void StartPlayAsync();
    void EndListenAsync();
    void EndPlayAsync();

    void WriteOutputData(const QByteArray& data);

Q_SIGNALS:
    void startListen();
    void startPlay();
    void endListen();
    void endPlay();

    void audioInput(QByteArray data);
    void audioOutput(QByteArray data);
    
protected:
    void timerEvent(QTimerEvent* event) override;

private:
    uint64_t AvgVolume(const QByteArray& data);


    int                                     _inTimer = 0;
    QAudioInput*                            _audioInput;
    QAudioOutput*                           _audioOutput;
    QIODevice*                              _ioInput;
    QIODevice*                              _ioOutput;
    QByteArray                              _inBufferData;
    int                                     _inLen;
    std::chrono::steady_clock::time_point   _inlastTick;
};

