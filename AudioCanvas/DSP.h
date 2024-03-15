#pragma once

#include <fftw3.h>
#include <miniaudio.h>
#include <iostream>
#include <string>
#include <vector>
#include "GlobalConstants.h"

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
	// FFTW3
	std::vector<std::vector<std::vector<double>>> FFT();
	int Run(std::vector<std::vector<float>>* data1, std::vector<std::vector<float>>* data2);
	// MISC
	void Window();
	// void Window(std::vector<std::vector<ma_int16>*>* CtoW, std::vector<std::vector<ma_int16>*>* CtoW2);

private:
	// MINIAUDIO
	ma_decoder decoder;
	ma_decoder_config decoderConfig;
	ma_uint64 frameCountTotal;
	// DATA
	const int N = CHUNK_SIZE;
	int numChunks;
	std::vector<ma_int16>* pSampleFrames;
	std::vector<ma_int16>* pChannel01 = new std::vector<ma_int16>();
	std::vector<ma_int16>* pChannel02 = new std::vector<ma_int16>();
	int timeChunks = 0;
	// consider not hardcoding 2 channels
	std::vector<std::vector<ma_int16>*>* pChannel01Chunked = new std::vector<std::vector<ma_int16>*>();
	std::vector<std::vector<ma_int16>*>* pChannel02Chunked = new std::vector<std::vector<ma_int16>*>();
	// FFTW3
	fftw_complex* in, * out;
	fftw_plan p;
	fftw_complex* in2, * out2;
	fftw_plan p2;
	//std::vector<std::vector<double>> result;

	// Filename
	std::string mFileName;
};