#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <fftw3.h>

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

    ma_decoder_config decoderConfig = ma_decoder_config_init(ma_format_s16, 2, 44100);
    decoderConfig.encodingFormat = ma_encoding_format_wav;

    ma_decoder_init_file("song-through-a-cardboard-world.wav", &decoderConfig, &decoder);

    ma_uint64 frameCountTotal;
    ma_decoder_get_length_in_pcm_frames(&decoder, &frameCountTotal);

    std::vector<ma_int16>* pSampleFrames = new std::vector<ma_int16>(frameCountTotal * 2);

    ma_decoder_read_pcm_frames(&decoder, pSampleFrames->data(), frameCountTotal, NULL);

    std::vector<ma_int16>* pChannel01 = new std::vector<ma_int16>();
    std::vector<ma_int16>* pChannel02 = new std::vector<ma_int16>();

    for (size_t i = 0; i < pSampleFrames->size(); i++) {
        if (i % 2 == 0) {
            pChannel01->push_back(pSampleFrames->at(i));
        } else {
            pChannel02->push_back(pSampleFrames->at(i));
        }
    }

    int timeChunks = ceil(frameCountTotal / 4410.0) - 1;

    std::vector<std::vector<ma_int16>*>* pChannel01Chunked = new std::vector<std::vector<ma_int16>*>();
    std::vector<std::vector<ma_int16>*>* pChannel02Chunked = new std::vector<std::vector<ma_int16>*>();

    for (int i = 0; i < timeChunks; i++) {
        std::vector<ma_int16>* tChannel01Chunk = new std::vector<ma_int16>();
        std::vector<ma_int16>* tChannel02Chunk = new std::vector<ma_int16>();
        for (int j = 0; j < 4410; j++) {
            tChannel01Chunk->push_back(pChannel01->at(size_t(i) * 4410 + j));
            tChannel02Chunk->push_back(pChannel02->at(size_t(i) * 4410 + j));
        }
        pChannel01Chunked->push_back(tChannel01Chunk);
		pChannel02Chunked->push_back(tChannel02Chunk);
    }

    int N = 4410;
    fftw_complex *in, * out;
    fftw_plan p;

    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

    for (size_t i = 0; i < N; i++) {
        in[i][0] = pChannel01Chunked->at(0)->at(i);
        in[i][1] = 0.0;
    }

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(p);

    for (int i = 0; i < N / 2; ++i) { // Only iterate over the first half of the output
        double real = out[i][0];
        double imag = out[i][1];
        double magnitude = sqrt(real * real + imag * imag);
        double frequency = i * 44100 / N;

        // Example: print magnitude and frequency for each bin
        std::cout << "Bin " << i << ": Frequency = " << frequency << " Hz, Magnitude = " << magnitude << std::endl;
    }

    delete pChannel01;
    delete pChannel02;

    delete pSampleFrames;

    ma_decoder_uninit(&decoder);

    return RunRenderer();
}