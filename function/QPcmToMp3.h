#pragma once

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/common.h>
    #include <libavutil/frame.h>
    #include <libavutil/samplefmt.h>
    #include <libavcodec/codec.h>
}

#include <QString>
#include <QByteArray>

class QPcmToMp3
{
public:
    QPcmToMp3();
    ~QPcmToMp3();

    bool Initialize(int32_t sampleRate, AVSampleFormat format, int32_t channels);
    bool TransformToMp3(const QString& savePath, QByteArray& data);
    void Clear();

private:
    int32_t                 _sampleRate = 0;
    AVSampleFormat          _format = {};
    int32_t                 _channels = 0;
    const AVCodec*          _codec = nullptr;
    AVCodecContext*         _codecContext = nullptr;
    AVFrame*                _frame = nullptr;
    AVPacket*               _pkt = nullptr;
};
