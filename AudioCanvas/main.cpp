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
int RunDSP() {
    DSP dsp;
    dsp.InitDecoder();
    dsp.InitData();
    dsp.InitFFT();
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

    for (int i = 0; i < 2; i++) { // iterate thru half data, contains {channel1, channel2}
        for (int j = 0; j < 4410 / 2; j++) {
            real1 = c1Data[i][j][0];
            imag1 = c1Data[i][j][1];
            magnitude1 = sqrt(real1 * real1 + imag1 * imag1);
            frequency1 = j * 44100 / 4410;
            std::cout << "Chunk " << i << " Bin " << j << ": Frequency1 = " << frequency1 << " Hz, Magnitude1 = " << magnitude1 << std::endl;

            real2 = c2Data[i + (dataSize / 2)][j][0]; // this is a weird way to access the data
            imag2 = c2Data[i + (dataSize / 2)][j][1]; // but should work based off of the symmetric property
            magnitude2 = sqrt(real2 * real2 + imag2 * imag2);
            frequency2 = j * 44100 / 4410;
            std::cout << "Chunk " << i << " Bin " << j << ": Frequency2 = " << frequency2 << " Hz, Magnitude2 = " << magnitude2 << std::endl;
        }
    }

    return 0;
}

int main() {
    {
        DSP dsp;
        dsp.InitDecoder();
        dsp.InitData();
        dsp.InitFFT();
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

        Renderer gRenderer;

        gRenderer.SetChannelData(&c1Data, &c2Data);

        gRenderer.Start();
    }

    return 0;
}