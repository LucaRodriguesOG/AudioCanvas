#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <limits>

#include "DSP.h"
#include "Renderer.h"

int RunRenderer() {
	{
		Renderer gRenderer;

		gRenderer.Start();
	}

	return 0;
}
int RunDSP(std::vector<std::vector<float>>& data1, std::vector<std::vector<float>>& data2) {
	DSP dsp;
	dsp.InitDecoder();
	dsp.InitData();
	//dsp.InitFFT();

	std::vector<fftw_complex*> c1Data;
	std::vector<fftw_complex*> c2Data;

	dsp.FFT(c1Data, c2Data);

	double real1;
	double imag1;
	double magnitude1;
	double frequency1;

	double real2;
	double imag2;
	double magnitude2;
	double frequency2;

	int dataSize = c1Data.size();

	double max = 0.0;

	for (int i = 0; i < c1Data.size(); i++) { // c1Data.size() for full song
		for (int j = 0; j < 1024 / 2; j++) { // iterate thru half data, contains {channel1, channel2}
			real1 = c1Data[i][j][0]; // index 1: chunk, index 2: bin, index 3: real/imag
			imag1 = c1Data[i][j][1];
			magnitude1 = sqrt(real1 * real1 + imag1 * imag1);
			frequency1 = j * 44100 / 1024;
			//std::cout << "Chunk " << i << " Bin " << j << ": Frequency1 = " << frequency1 << " Hz, Magnitude1 = " << magnitude1 << std::endl;

			//real2 = c2Data[i + (dataSize / 2)][j][0]; // this is a weird way to access the data
			//imag2 = c2Data[i + (dataSize / 2)][j][1]; // but should work based off of the symmetric property
			real2 = c2Data[i][j][0];
			imag2 = c2Data[i][j][1];
			magnitude2 = sqrt(real2 * real2 + imag2 * imag2);
			frequency2 = j * 44100 / 1024;
			//std::cout << "Chunk " << i << " Bin " << j << ": Frequency2 = " << frequency2 << " Hz, Magnitude2 = " << magnitude2 << std::endl;

			if (magnitude1 > max) { // find max of all magnitudes
				max = magnitude1;
				if (magnitude2 > max) {
					max = magnitude2;
				}
			}
		}
	}
	std::cout << "MAX MAGNITUDE: " << max << std::endl;

	for (size_t i = 0; i < c1Data.size(); i++) {
		std::vector<float> tempBins(1024);
		for (size_t j = 0; j < 1024; j++) {
			double realSquared = c1Data[i][j][0] * c1Data[i][j][0];
			double imaginarySquared = c1Data[i][j][1] * c1Data[i][j][1];
			tempBins.at(j) = sqrt(realSquared + imaginarySquared) / max;
		}
		data1.push_back(tempBins);
	}


	for (size_t i = 0; i < c2Data.size(); i++) {
		std::vector<float> tempBins(1024);
		for (size_t j = 0; j < 1024; j++) {
			double realSquared = c2Data[i][j][0] * c2Data[i][j][0];
			double imaginarySquared = c2Data[i][j][1] * c2Data[i][j][1];
			tempBins.at(j) = sqrt(realSquared + imaginarySquared) / max;
		}
		data2.push_back(tempBins);
	}

	return 0;
}

int main() {
	{
		std::vector<std::vector<float>> c1Data;
		std::vector<std::vector<float>> c2Data;

		RunDSP(c1Data, c2Data);

		ma_result result;
		ma_engine engine;

		result = ma_engine_init(NULL, &engine);
		if (result != MA_SUCCESS)
			std::cout << "Music Engine failed to initialize!" << std::endl;

		Renderer gRenderer;

		gRenderer.SetChannelData(&c1Data, &c2Data);

		ma_engine_play_sound(&engine, "song-through-a-cardboard-world.wav", NULL);
		ma_engine_set_volume(&engine, 0.1f);

		gRenderer.Start();

		
	}

	return 0;
}