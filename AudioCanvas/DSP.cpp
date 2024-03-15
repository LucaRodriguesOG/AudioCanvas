#include "DSP.h"

DSP::DSP() {
	mFileName = "song-through-a-cardboard-world.wav";
	InitDecoder();
	InitData();
}

DSP::DSP(std::string FileName) : mFileName(FileName) {
	InitDecoder();
	InitData();
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

	timeChunks = ceil(frameCountTotal / double(CHUNK_SIZE)) - 1; // frames / 100 ms chunks

	for (int i = 0; i < timeChunks; i++) {
		std::vector<ma_int16>* tChannel01Chunk = new std::vector<ma_int16>();
		std::vector<ma_int16>* tChannel02Chunk = new std::vector<ma_int16>();
		for (int j = 0; j < CHUNK_SIZE; j++) { // create chunks from sample data
			tChannel01Chunk->push_back(pChannel01->at(size_t(i) * CHUNK_SIZE + j));
			tChannel02Chunk->push_back(pChannel02->at(size_t(i) * CHUNK_SIZE + j));
		}
		pChannel01Chunked->push_back(tChannel01Chunk); // put chunks in channel arrays
		pChannel02Chunked->push_back(tChannel02Chunk);
	}
	numChunks = pChannel01Chunked->size();

	Window();
}

std::vector<std::vector<std::vector<double>>> DSP::FFT() {

	std::vector<std::vector<std::vector<double>>> result(numChunks, std::vector<std::vector<double>>(N, std::vector<double>(2)));
	fftw_complex* in = new fftw_complex[CHUNK_SIZE];
	fftw_complex* out = new fftw_complex[CHUNK_SIZE];
	fftw_plan p;
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	for (int i = 0; i < numChunks; i++) { // for every chunk we have
		for (int j = 0; j < N; j++) { // for every sample we have
			in[j][0] = pChannel01Chunked->at(i)->at(j); // for current chunk, populate input with all samples
			in[j][1] = 0;
		}

		fftw_execute(p);

		for (int j = 0; j < N; j++) { // for every bin we have (out bin # is directly correlated with in samples)
			result.at(i).at(j).at(0) = out[j][0]; // for current chunk, populate result with real and imag parts
			result.at(i).at(j).at(1) = out[j][1];
		}
	}

	fftw_destroy_plan(p);
	fftw_cleanup();

	return result;
}

int DSP::Run(std::vector<std::vector<float>>* data1, std::vector<std::vector<float>>* data2) {
	std::vector<std::vector<std::vector<double>>> fft;
	//std::vector<std::vector<std::vector<double>>> fft2;
	
	fft = this->FFT();

	double real1;
	double imag1;
	double magnitude1;

	//double real2;
	//double imag2;
	//double magnitude2;

	double frequency;

	int dataSize = fft.size();

	double max = 0.0;

	for (int i = 0; i < fft.size(); i++) { // c1Data.size() for full song
		for (int j = 0; j < CHUNK_SIZE / 2; j++) { // iterate thru half data, contains {channel1, channel2}
			real1 = fft[i][j][0]; // index 1: chunk, index 2: bin, index 3: real/imag
			imag1 = fft[i][j][1];
			magnitude1 = sqrt(real1 * real1 + imag1 * imag1);
			//frequency = j * 44100 / CHUNK_SIZE;
			//std::cout << "Chunk " << i << " Bin " << j << ": Frequency1 = " << frequency << " Hz, Magnitude1 = " << magnitude1 << std::endl;

			//real2 = c2Data[i][j][0];
			//imag2 = c2Data[i][j][1];
			//magnitude2 = sqrt(real2 * real2 + imag2 * imag2);
			//frequency = j * 44100 / CHUNK_SIZE;
			//std::cout << "Chunk " << i << " Bin " << j << ": Frequency2 = " << frequency << " Hz, Magnitude2 = " << magnitude2 << std::endl;

			if (magnitude1 > max) { // find max of all magnitudes
				max = magnitude1;
				/*if (magnitude2 > max) {
					max = magnitude2;
				}*/
			}
		}
	}
	std::cout << "MAX MAGNITUDE: " << max << std::endl;

	for (size_t i = 0; i < fft.size(); i++) {
		std::vector<float> tempBins(CHUNK_SIZE);
		for (size_t j = 0; j < CHUNK_SIZE; j++) {
			double realSquared = fft[i][j][0] * fft[i][j][0];
			double imaginarySquared = fft[i][j][1] * fft[i][j][1];
			tempBins.at(j) = sqrt(realSquared + imaginarySquared) / max;
		}
		data1->push_back(tempBins);
	}


	/*for (size_t i = 0; i < c2Data.size(); i++) {
		std::vector<float> tempBins(CHUNK_SIZE);
		for (size_t j = 0; j < CHUNK_SIZE; j++) {
			double realSquared = c2Data[i][j][0] * c2Data[i][j][0];
			double imaginarySquared = c2Data[i][j][1] * c2Data[i][j][1];
			tempBins.at(j) = sqrt(realSquared + imaginarySquared) / max;
		}
		data2->push_back(tempBins);
	}*/
	return 0;
}

void DSP::Window() { // hann window function 
	for (int i = 0; i < pChannel01Chunked->size(); i++) {
		for (int j = 0; j < CHUNK_SIZE / 2; j++) {
			pChannel01Chunked->at(i)->at(j) = ma_int16(int(pChannel01Chunked->at(i)->at(j)) * (cos(3.1415926535 * j / CHUNK_SIZE) * cos(3.1415926535 * j / CHUNK_SIZE) / CHUNK_SIZE));
			pChannel01Chunked->at(i)->at(j + (CHUNK_SIZE / 2)) = 0;
			//pChannel01Chunked->at(i)->at(j) = ma_int16(int(pChannel01Chunked->at(i)->at(j)) * (0.53836 * (0.46164 - cos(2.0 * 3.1415926535 * j / (CHUNK_SIZE - 1))))); // hamming window function (hann has 0.5 for both)
		}
	}
}