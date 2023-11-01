#pragma once

#include <QObject>
#include <QAudioDeviceInfo>
#include <QThread>
#include <QTimerEvent>
#include <WRL.h>

#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <Audiopolicy.h>

class AudioLoop : public QObject
{
    Q_OBJECT
public:
    AudioLoop();
    ~AudioLoop();

    void Initialize();
    void Uninitialize();

    void StartMic();
    void StartMic(const QAudioDeviceInfo& info);
    void EndMic();
    bool IsRunning();

    static void InitCom();
    static void UnintCom();

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
    IMMDevice* SelectAudioClient();

    int32_t AvgDb(const QByteArray& data);

    QThread                                     _workThread;
    QAudioDeviceInfo                            _devInfo;
    QByteArray                                  _bufferData;
    Microsoft::WRL::ComPtr<IMMDeviceEnumerator> _enumerator;
    Microsoft::WRL::ComPtr<IMMDevice>           _endpoint;
    Microsoft::WRL::ComPtr<IAudioClient>        _audioClient;
    Microsoft::WRL::ComPtr<IAudioCaptureClient> _captureClient;
    WAVEFORMATEX                                _format = {};
    int                                         _timer = 0;
    std::chrono::steady_clock::time_point       _lastTick;
};
