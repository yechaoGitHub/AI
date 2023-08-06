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

    void StartReadMic();
    void StartReadSpeaker();
    void EndReadMic();
    void EndReadSpeaker();

    void WriteOutputData(const QByteArray& data);

Q_SIGNALS:
    void audioInput(QByteArray data);
    void audioOutput(QByteArray data);
    
protected:
    void timerEvent(QTimerEvent* event) override;

private:
    void ReadAudioData(QIODevice* dev, int& readLen, QByteArray& bufferData, std::chrono::steady_clock::time_point& timePoint, void(Audio::*sginal)(QByteArray));

    uint64_t AvgVolume(const QByteArray& data);

    QAudioInput*                            _audioInput = nullptr;
    QAudioOutput*                           _audioOutput = nullptr;

#pragma region Âó¿Ë·ç
    QIODevice*                              _ioInput;
    int                                     _inTimer = 0;
    QByteArray                              _inBufferData;
    int                                     _inLen = 0;
    std::chrono::steady_clock::time_point   _inlastTick;
#pragma endregion

#pragma region ÑïÉùÆ÷
    QIODevice*                              _ioOutput;
    int                                     _outTimer = 0;
    QByteArray                              _outBufferData;
    int                                     _outLen = 0;
    std::chrono::steady_clock::time_point   _outlastTick;
#pragma endregion

};

