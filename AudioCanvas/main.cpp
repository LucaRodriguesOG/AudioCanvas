#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "DSP.h"
#include "Renderer.h"

int RunRenderer() {
    {
        Renderer gRenderer;

        gRenderer.Start();
    }

    return 0;
}

int main() {
    DSP dsp;
    dsp.InitDecoder();
    dsp.InitData();
    dsp.InitFFT();
    
    fftw_complex* out = dsp.FFT();

    for (int i = 0; i < 4410 / 2; ++i) { // Only iterate over the first half of the output
        double real = out[i][0];
        double imag = out[i][1];
        double magnitude = sqrt(real * real + imag * imag);
        double frequency = i * 44100 / 4410;

        // Example: print magnitude and frequency for each bin
        std::cout << "Bin " << i << ": Frequency = " << frequency << " Hz, Magnitude = " << magnitude << std::endl;
    }

    return RunRenderer();
}