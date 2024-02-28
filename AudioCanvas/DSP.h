#pragma once

#include <fftw3.h>
#include <miniaudio.h>
#include <iostream>
#include <string>
#include <vector>

class DSP {
public:
	DSP();
	DSP(std::string FileName);
	~DSP();
	// MINIAUDIO
	void InitDecoder();
	void InitData();
	void InitFFT();

	// TODO: Change FFT function to instead output floats of the complex's magnitude 
	// Still going to be a vector of arrays. But the arrays will no longer be of type fftw_complex.
	// They will just be of float (which will represent the magnitude. Also, the magnitude will
	// be normalized. Find magnitude by finding local max of dataset.

	void FFT(std::vector<fftw_complex*>& c1, std::vector<fftw_complex*>& c2);

private:
	// MINIAUDIO
	ma_decoder decoder;
	ma_decoder_config decoderConfig;
	ma_uint64 frameCountTotal;
	// DATA
	// consider putting this as a param for different time chunks
	int N = 4410;
	int numChunks = 0;
	std::vector<ma_int16>* pSampleFrames;
	std::vector<ma_int16>* pChannel01 = new std::vector<ma_int16>();
	std::vector<ma_int16>* pChannel02 = new std::vector<ma_int16>();
	int timeChunks = ceil(frameCountTotal / 4410.0) - 1;
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