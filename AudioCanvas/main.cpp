#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <fftw3.h>

#include "Renderer.h"
#include <mutex>

const int gDataSize = 441 / 2 + 1;
std::vector<float> gData(gDataSize, 0.5);
std::mutex gDataMutex;

Renderer gRenderer(&gData, &gDataMutex);

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	std::lock_guard<std::mutex> lock(gDataMutex);
	const float* samples = (const float*)pInput;

	const int N = 441; // Size of the array
	double in[N]; // Input array
	fftw_complex out[N / 2 + 1]; // Output array
	fftw_plan p;

	// Initialize the input data with some values
	for (int i = 0; i < N; i++) {
		in[i] = samples[i];
	}

	// Create a plan for a forward DFT
	p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);

	// Execute the plan
	fftw_execute(p);

	std::pair<int, double> maxFreqMagnitude(0, 0.0);
	double sum = 0.0;
	for (int i = 0; i < N / 2 + 1; i++) {
		// Calculate magnitude of the complex number
		double magnitude = sqrt(pow(out[i][0], 2) + pow(out[i][1], 2));

		// Update if this frequency has a higher magnitude than the current max
		if (magnitude > maxFreqMagnitude.second) {
			maxFreqMagnitude.first = i; // Frequency index
			maxFreqMagnitude.second = magnitude; // Magnitude
		}
		
		sum += magnitude;

		gData[i] = magnitude;
	}

	for (size_t i = 0; i < gData.size(); i++) {
		gData[i] /= sqrt(sum);
	}

	double maxFrequencyHz = static_cast<double>(maxFreqMagnitude.first) * 44100 / N;
	std::cout << "Frequency bin with max magnitude: " << maxFrequencyHz
		<< " (Magnitude: " << maxFreqMagnitude.second << ")" << std::endl;

	// Destroy the plan
	fftw_destroy_plan(p);
}

int main() {
	ma_result result;
	ma_context context;
	ma_device_config deviceConfig;
	ma_device device;

	result = ma_context_init(NULL, 0, NULL, &context);
	if (result != MA_SUCCESS) {
		return -1;
	}

	deviceConfig = ma_device_config_init(ma_device_type_loopback);
	deviceConfig.capture.format = ma_format_f32; // or your preferred format
	deviceConfig.capture.channels = 1; // mono
	deviceConfig.sampleRate = 44100; // or your preferred sample rate
	deviceConfig.dataCallback = data_callback;
	// No need to set a playback format for loopback.

	result = ma_device_init(&context, &deviceConfig, &device);
	if (result != MA_SUCCESS) {
		ma_context_uninit(&context);
		return -2;
	}

	result = ma_device_start(&device);
	if (result != MA_SUCCESS) {
		ma_device_uninit(&device);
		ma_context_uninit(&context);
		return -3;
	}

	gRenderer.Start();

	ma_device_uninit(&device);
	ma_context_uninit(&context);

	return 0;

	return 0;
}