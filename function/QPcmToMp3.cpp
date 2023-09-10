#include "QPcmToMp3.h"
#include <fstream>

QPcmToMp3::QPcmToMp3()
{
}

QPcmToMp3::~QPcmToMp3()
{
    Clear();
}

bool QPcmToMp3::Initialize(int32_t sampleRate, AVSampleFormat format, int32_t channels)
{
    _codec = avcodec_find_encoder(AV_CODEC_ID_MP3);
    if (!_codec)
    {
        return false;
    }

    _sampleRate = sampleRate;
    _format = format;
    _channels = channels;

    return true;
}

bool QPcmToMp3::TransformToMp3(const QString& savePath, QByteArray& data)
{
    if (data.isEmpty())
    {
        return false;
    }

    _codecContext = avcodec_alloc_context3(_codec);
    if (!_codecContext)
    {
        return false;
    }

    _codecContext->bit_rate = 64000;
    _codecContext->time_base = av_get_time_base_q();
    _codecContext->sample_fmt = _format;
    _codecContext->sample_rate = _sampleRate;
    av_channel_layout_default(&_codecContext->ch_layout, _channels);

    if (avcodec_open2(_codecContext, _codec, nullptr) < 0)
    {
        return false;
    }

    _frame = av_frame_alloc();
    if (!_frame)
    {
        return false;
    }

    _frame->nb_samples = _codecContext->frame_size;
    _frame->format = _codecContext->sample_fmt;
    auto ret = av_channel_layout_copy(&_frame->ch_layout, &_codecContext->ch_layout);
    if (ret < 0)
    {
        return false;
    }

    ret = av_frame_get_buffer(_frame, 0);
    if (ret < 0)
    {
        return false;
    }

    _pkt = av_packet_alloc();
    if (!_pkt)
    {
        return false;
    }

    std::fstream f;
    f.open(savePath.toLocal8Bit().data(), std::fstream::out | std::fstream::binary);
    if (!f)
    {
        Clear();
        return false;
    }

    QByteArray copyData;
    uint8_t* intputData{};
    int64_t r = 0;
    auto c = av_get_bytes_per_sample(_format);
    int64_t frameSize = _frame->nb_samples * c;
    while (r < data.size())
    {
        auto ret = av_frame_make_writable(_frame);
        if (ret < 0)
        {
            Clear();
            return false;
        }

        int64_t remain = static_cast<int64_t>(data.size()) - r;
        if (remain >= frameSize)
        {
            intputData = reinterpret_cast<uint8_t*>(data.data()) + r;
            r += frameSize;
        }
        else
        {
            copyData.append(_codecContext->sample_rate, 0);

            auto src = data.data() + r;
            ::memcpy(copyData.data(), src, remain);
            intputData = reinterpret_cast<uint8_t*>(copyData.data());

            r += remain;
        }

        for (auto i = 0; i < _codecContext->ch_layout.nb_channels; i++)
        {
            _frame->data[i] = intputData;
        }

        ret = avcodec_send_frame(_codecContext, _frame);
        if (ret < 0)
        {
            Clear();
            return false;
        }

        while (ret >= 0)
        {
            ret = avcodec_receive_packet(_codecContext, _pkt);
            if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
            {
                continue;
            }
            else if (ret < 0)
            {
                break;
            }
            f.write((const char*)_pkt->data, _pkt->size);
            av_packet_unref(_pkt);
        }
    }

    ret = avcodec_send_frame(_codecContext, nullptr);
    while (ret >= 0)
    {
        ret = avcodec_receive_packet(_codecContext, _pkt);
        if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
        {
            continue;
        }
        else if (ret < 0)
        {
            break;
        }
        f.write((const char*)_pkt->data, _pkt->size);
        av_packet_unref(_pkt);
    }

    f.close();

    Clear();
    return true;
}

void QPcmToMp3::Clear()
{
    if (_frame)
    {
        av_frame_free(&_frame);
        _frame = nullptr;
    }

    if (_pkt)
    {
        av_packet_free(&_pkt);
        _pkt = nullptr;
    }

    if (_codecContext)
    {
        avcodec_free_context(&_codecContext);
        _codecContext = nullptr;
    }
}
