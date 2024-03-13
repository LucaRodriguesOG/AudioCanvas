#include "DSP.h"

DSP::DSP() {
	mFileName = "test-500left-2000right.wav";
}

DSP::DSP(std::string FileName) : mFileName(FileName) {

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

	ma_decoder_init_file(mFileName.c_str(), NULL, &decoder);
}

void DSP::InitData() {
	ma_decoder_get_length_in_pcm_frames(&decoder, &frameCountTotal);

	pSampleFrames = new std::vector<ma_int16>(frameCountTotal * 2); // 2 is for ChannelNum

	ma_decoder_read_pcm_frames(&decoder, pSampleFrames->data(), frameCountTotal, NULL);

	for (size_t i = 0; i < pSampleFrames->size(); i++) { // split data into its two channels
		if (i % 2 == 0) {
			pChannel01->push_back(pSampleFrames->at(i));
		}
		else {
			pChannel02->push_back(pSampleFrames->at(i));
		}
	}

	int timeChunks = ceil(frameCountTotal / 4410.0) - 1; // frames / 100 ms chunks

	for (int i = 0; i < timeChunks; i++) {
		std::vector<ma_int16>* tChannel01Chunk = new std::vector<ma_int16>();
		std::vector<ma_int16>* tChannel02Chunk = new std::vector<ma_int16>();
		for (int j = 0; j < 4410; j++) { // create chunks from sample data
			tChannel01Chunk->push_back(pChannel01->at(size_t(i) * 4410 + j));
			tChannel02Chunk->push_back(pChannel02->at(size_t(i) * 4410 + j));
		}
		pChannel01Chunked->push_back(tChannel01Chunk); // put chunks in channel arrays
		pChannel02Chunked->push_back(tChannel02Chunk);
	}
	numChunks = pChannel01Chunked->size();
}

void DSP::InitFFT() { // standard fft initialization
	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	in2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
	out2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
	p2 = fftw_plan_dft_1d(N, in2, out2, FFTW_FORWARD, FFTW_ESTIMATE);
}

void DSP::FFT(std::vector<fftw_complex*>& c1, std::vector<fftw_complex*>& c2) {

	for (size_t i = 0; i < numChunks; i++) { // create channel 1 input
		for (size_t j = 0; j < N; j++) {
			in[j][0] = pChannel01Chunked->at(i)->at(j);
			in[j][1] = 0.0;
		}
		fftw_execute(p); // execute FFT on channel 2
		c1.push_back(out); // put output in c1
	}

	for (size_t i = 0; i < numChunks; i++) { // create channel 2 input
		for (size_t j = 0; j < N; j++) {
			in2[j][0] = pChannel02Chunked->at(i)->at(j);
			in2[j][1] = 0.0;
		}
		fftw_execute(p2); // execute FFt on channel 2
		c2.push_back(out2); // put output in c2
	}
}