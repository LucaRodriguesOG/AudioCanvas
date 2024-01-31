#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "Renderer.h"

int RunRenderer() {
    {
        Renderer gRenderer;

        gRenderer.Start();
    }

    return 0;
}

int main() {
    ma_decoder decoder;

    ma_decoder_config decoderConfig = ma_decoder_config_init(ma_format_s16, 2, 0);
    decoderConfig.encodingFormat = ma_encoding_format_wav;

    ma_decoder_init_file("song-through-a-cardboard-world.wav", &decoderConfig, &decoder);

    ma_uint64 frameCountTotal;
    ma_decoder_get_length_in_pcm_frames(&decoder, &frameCountTotal);

    std::vector<ma_int16>* pSampleFrames = new std::vector<ma_int16>(frameCountTotal * 2);

    ma_decoder_read_pcm_frames(&decoder, pSampleFrames->data(), frameCountTotal, NULL);

    delete pSampleFrames;

    ma_decoder_uninit(&decoder);

    return RunRenderer();
}