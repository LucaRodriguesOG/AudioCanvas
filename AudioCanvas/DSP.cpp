#include "DSP.h"

DSP::DSP() {

}

DSP::~DSP() {
	delete pChannel01;
	delete pChannel02;

	delete pSampleFrames;

	ma_decoder_uninit(&decoder);
}

void DSP::InitDecoder() {
	decoderConfig = ma_decoder_config_init(ma_format_s16, 2, 44100);
	decoderConfig.encodingFormat = ma_encoding_format_wav;

	ma_decoder_init_file("song-through-a-cardboard-world.wav", &decoderConfig, &decoder);
}

void DSP::InitData() {
	ma_decoder_get_length_in_pcm_frames(&decoder, &frameCountTotal);

	pSampleFrames = new std::vector<ma_int16>(frameCountTotal * 2); // 2 is for ChannelNum

	ma_decoder_read_pcm_frames(&decoder, pSampleFrames->data(), frameCountTotal, NULL);

	for (size_t i = 0; i < pSampleFrames->size(); i++) {
		if (i % 2 == 0) {
			pChannel01->push_back(pSampleFrames->at(i));
		}
		else {
			pChannel02->push_back(pSampleFrames->at(i));
		}
	}

	int timeChunks = ceil(frameCountTotal / 4410.0) - 1;

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
}

void DSP::InitFFT() {
	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	for (size_t i = 0; i < N; i++) {
		in[i][0] = pChannel01Chunked->at(0)->at(i);
		in[i][1] = 0.0;
	}
}

fftw_complex* DSP::FFT() {

	fftw_execute(p);

	return out;
}