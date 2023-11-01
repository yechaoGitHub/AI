#include "AudioLoop.h"
#include "functiondiscoverykeys_devpkey.h"

AudioLoop::AudioLoop()
{
}

AudioLoop::~AudioLoop()
{
}

void AudioLoop::Initialize()
{
    auto hr = ::CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)_enumerator.GetAddressOf());
    this->moveToThread(&_workThread);
}

void AudioLoop::Uninitialize()
{
    if (_workThread.isRunning())
    {
        EndMic();
    }
}

void AudioLoop::StartMic()
{
    if (_workThread.isRunning())
    {
        return;
    }

    _workThread.start();

    connect(this, &AudioLoop::start_mic, this, &AudioLoop::StartMicInternal);
    connect(this, &AudioLoop::end_mic, this, &AudioLoop::EndMicInternal);

    emit start_mic();
}

void AudioLoop::StartMic(const QAudioDeviceInfo& info)
{
    if (_workThread.isRunning())
    {
        return;
    }

    _devInfo = info;
    _workThread.start();

    connect(this, &AudioLoop::start_mic, this, &AudioLoop::StartMicInternal);
    connect(this, &AudioLoop::end_mic, this, &AudioLoop::EndMicInternal);

    emit start_mic();
}

void AudioLoop::EndMic()
{
    if (_workThread.isRunning())
    {
        emit end_mic();
        if (!_workThread.wait(1000))
        {
            _workThread.quit();
        }

        disconnect(this, &AudioLoop::start_mic, this, &AudioLoop::StartMicInternal);
        disconnect(this, &AudioLoop::end_mic, this, &AudioLoop::EndMicInternal);

        emit soundPlay(false);
    }
}

bool AudioLoop::IsRunning()
{
    return _workThread.isRunning();
}

void AudioLoop::InitCom()
{
    CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED);
}

void AudioLoop::UnintCom()
{
    CoUninitialize();
}

void AudioLoop::timerEvent(QTimerEvent* event)
{
    BYTE* pData{};                  // samples 数据
    UINT32 numFramesAvailable{};    // 缓冲区有多少帧
    DWORD dwFlags{};

    auto hr = _captureClient->GetBuffer(&pData, &numFramesAvailable, &dwFlags, NULL, NULL);
    if (pData == nullptr || hr == AUDCLNT_S_BUFFER_EMPTY)
    {
        _captureClient->ReleaseBuffer(numFramesAvailable);
        return;
    }

    int frameBytes = _format.nChannels * _format.wBitsPerSample / 8;
    int count = numFramesAvailable * frameBytes;

    _bufferData.append((char*)pData, count);

    _captureClient->ReleaseBuffer(numFramesAvailable);

    if (_bufferData.size() >= 1280)
    {
        emit audioInput(std::move(_bufferData.mid(0, 1280)));
        emit soundPlay(true);
        _bufferData.remove(0, 1280);
    }
}

void AudioLoop::StartMicInternal()
{
    Microsoft::WRL::ComPtr<IMMDeviceCollection> pColletion;

    if (!_enumerator)
    {
        goto error;
    }

    _endpoint = SelectAudioClient();
    if (_endpoint == nullptr)
    {
        goto error;
    }

    auto hr = _endpoint->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)_audioClient.GetAddressOf());
    if (FAILED(hr))
    {
        goto error;
    }

    _format.cbSize = 0;
    _format.wFormatTag = WAVE_FORMAT_PCM;
    _format.nChannels = 1;
    _format.nSamplesPerSec = 16000;
    _format.wBitsPerSample = 16;
    _format.nBlockAlign = _format.nChannels * _format.wBitsPerSample / 8;
    _format.nAvgBytesPerSec = _format.nSamplesPerSec * _format.nBlockAlign;

    constexpr int REFTIMES_PER_SEC = 10000000;      // 1 reference_time = 100ns
    // 初始化 IAudioClient 对象
    const REFERENCE_TIME hnsRequestedDuration = 1 * REFTIMES_PER_SEC; // 1s
    hr = _audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration, 0, &_format, nullptr);
    if (FAILED(hr))
    {
        goto error;
    }

    hr = _audioClient->GetService(__uuidof(IAudioCaptureClient), (void**)_captureClient.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        goto error;
    }

    hr = _audioClient->Start();
    if (FAILED(hr))
    {
        goto error;
    }

    _timer = startTimer(1, Qt::PreciseTimer);
    return;

error:
    EndMicInternal();
}

void AudioLoop::EndMicInternal()
{
    if (_audioClient)
    {
        _audioClient->Stop();
    }

    if (_timer != 0)
    {
        killTimer(_timer);
    }

    _endpoint.Reset();
    _audioClient.Reset();
    _captureClient.Reset();

    _workThread.quit();
}

IMMDevice* AudioLoop::SelectAudioClient()
{
    auto devName = _devInfo.deviceName();

    if (_devInfo.deviceName() == "Default Output Device")
    {
        Microsoft::WRL::ComPtr<IMMDevice> pEndpoint;

        _enumerator->GetDefaultAudioEndpoint(eRender, eConsole, pEndpoint.GetAddressOf());

        return pEndpoint.Detach();
    }
    else
    {
        Microsoft::WRL::ComPtr<IMMDeviceCollection> pColletion;

        auto hr = _enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, pColletion.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        UINT count{};
        hr = pColletion->GetCount(&count);
        if (FAILED(hr))
        {
            return false;
        }

        for (UINT i = 0; i < count; ++i) {
            Microsoft::WRL::ComPtr<IMMDevice> pEndpoint;
            hr = pColletion->Item(i, pEndpoint.GetAddressOf());
            if (FAILED(hr))
            {
                return false;
            }

            //Microsoft::WRL::ComPtr<WCHAR> pwszID;
            //hr = pEndpoint->GetId(&pwszID);
            //exit_on_failed(hr);

            Microsoft::WRL::ComPtr<IPropertyStore> pProps;
            hr = pEndpoint->OpenPropertyStore(STGM_READ, pProps.GetAddressOf());
            if (FAILED(hr))
            {
                return false;
            }

            PROPVARIANT varName;
            PropVariantInit(&varName);
            hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
            if (FAILED(hr))
            {
                return false;
            }

            QString clientName = QString::fromWCharArray(varName.pwszVal);
            if (devName == clientName)
            {
                PropVariantClear(&varName);
                return pEndpoint.Detach();
            }

            PropVariantClear(&varName);
        }

        return nullptr;

    }
}

int32_t AudioLoop::AvgDb(const QByteArray& data)
{
    uint64_t total{};
    auto count = data.size() / 2;
    auto sample = reinterpret_cast<const uint16_t*>(data.data());
    for (auto i = 0; i < count; i++)
    {
        total += sample[i];
    }

    uint64_t avg = total / count;
    auto db = 20 * log10((double)avg / (double)UINT16_MAX);
    return db;
}
