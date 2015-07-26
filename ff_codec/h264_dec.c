
#include "libavcodec/h264.h"
#include "libavcodec/avcodec.h"
#include <stdio.h>
extern AVCodec ff_h264_decoder ;
int main(int argc,char*argv[])
{
#if 0
    if(argc<2){
        printf("not enough parmeter\n");
        return -1;
    }
    char* infile=argv[1];
    AVCodec *codec=&ff_h264_decoder;
    AVCodecContext tmp;
    AVCodecContext* avctx=&tmp;
    int ret;
    //avctx->codec=&ff_h264_decoder;
    if ((!codec && !avctx->codec)) {
        av_log(avctx, AV_LOG_ERROR, "No codec provided to avcodec_open2()\n");
        return AVERROR(EINVAL);
    }
    if ((codec && avctx->codec && codec != avctx->codec)) {
        av_log(avctx, AV_LOG_ERROR, "This AVCodecContext was allocated for %s, "
                                    "but %s passed to avcodec_open2()\n", avctx->codec->name, codec->name);
        return AVERROR(EINVAL);
    }
    if (!codec)
        codec = avctx->codec;

    if (avctx->extradata_size < 0 || avctx->extradata_size >= FF_MAX_EXTRADATA_SIZE)
        return AVERROR(EINVAL);

    //if (options)
    //    av_dict_copy(&tmp, *options, 0);

    ret = ff_lock_avcodec(avctx, codec);
    if (ret < 0)
        return ret;
    avctx->internal = av_mallocz(sizeof(AVCodecInternal));
    if (!avctx->internal) {
        ret = AVERROR(ENOMEM);
        goto end;
    }
    avctx->internal->pool = av_mallocz(sizeof(*avctx->internal->pool));
    if (!avctx->internal->pool) {
        ret = AVERROR(ENOMEM);
        goto free_and_end;
    }
    avctx->internal->to_free = av_frame_alloc();
    if (!avctx->internal->to_free) {
        ret = AVERROR(ENOMEM);
        goto free_and_end;
    }

    if (codec->priv_data_size > 0) {
        if (!avctx->priv_data) {
            avctx->priv_data = av_mallocz(codec->priv_data_size);
            if (!avctx->priv_data) {
                ret = AVERROR(ENOMEM);
                goto end;
            }
            if (codec->priv_class) {
                *(const AVClass **)avctx->priv_data = codec->priv_class;
                av_opt_set_defaults(avctx->priv_data);
            }
        }
        //if (codec->priv_class && (ret = av_opt_set_dict(avctx->priv_data, &tmp)) < 0)
        //    goto free_and_end;
    } else {
        avctx->priv_data = NULL;
    }

    avctx->codec = codec;
    avctx->codec_id=AV_CODEC_ID_H264;

    avctx->frame_number = 0;
    avctx->codec_descriptor = avcodec_descriptor_get(avctx->codec_id);
    
    //if(ff_h264_decode_init(avctx)) {
    if(avctx->codec->init(avctx)) {
        fprintf(stdout,"could not creat decoder\n");
        return -1;
    }
    ff_unlock_avcodec();

    {
    AVPacket pkt1, *pkt;
    int ret = 0, i, got_packet = 0;
    pkt=&pkt1;
    av_init_packet(pkt);

    }


    return 0;
end:
#endif
}