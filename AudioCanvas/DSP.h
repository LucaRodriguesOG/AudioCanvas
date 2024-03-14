#pragma once

#include <fftw3.h>
#include <miniaudio.h>
#include <iostream>
#include <string>
#include <vector>

/*

	REFACTOR:

	class Song {
		public:
			Song(string songLocation, double msDelay);	// Calculate chunks from msDelay

		private:
			vector<vector<float>> channel01Chunks;
			vector<vector<float>> channel02Chunks;
	}

*/

class DSP {
public:
	DSP();
	DSP(std::string FileName);
	~DSP();
	// MINIAUDIO
	void InitDecoder();
	void InitData();
	void InitFFT();
	void InitFFT2();
	void DeinitFFT();
	void DeinitFFT2();

	void FFT(std::vector<fftw_complex*>& c1, std::vector<fftw_complex*>& c2);

private:
	// MINIAUDIO
	ma_decoder decoder;
	ma_decoder_config decoderConfig;
	ma_uint64 frameCountTotal;
	// DATA
	// consider putting this as a param for different time chunks
	const int N = 1024;
	int numChunks = 0;
	std::vector<ma_int16>* pSampleFrames;
	std::vector<ma_int16>* pChannel01 = new std::vector<ma_int16>();
	std::vector<ma_int16>* pChannel02 = new std::vector<ma_int16>();
	int timeChunks = ceil(frameCountTotal / 1024.0) - 1;
	// consider not hardcoding 2 channels
	std::vector<std::vector<ma_int16>*>* pChannel01Chunked = new std::vector<std::vector<ma_int16>*>();
	std::vector<std::vector<ma_int16>*>* pChannel02Chunked = new std::vector<std::vector<ma_int16>*>();
	// FFTW3
	fftw_complex* in, * out;
	fftw_plan p;
	fftw_complex* in2, * out2;
	fftw_plan p2;

	// Filename
	std::string mFileName;
};