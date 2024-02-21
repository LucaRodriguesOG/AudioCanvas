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
    
    std::vector<fftw_complex*> transformedData = dsp.FFT();
    
    double real1;
    double imag1;
    double magnitude1;
    double frequency1;

    double real2;
    double imag2;
    double magnitude2;
    double frequency2;

    int dataSize = transformedData.size();

    for (int i = 0; i < dataSize / 4; i++) {
        std::cout << transformedData[i][0][0] << std::endl << transformedData[i + dataSize / 4][0][0] << std::endl << std::endl;
    }

    //for (int i = 0; i < dataSize / 4; i++) { // iterate thru half data, contains {channel1, channel2}
    //    for (int j = 0; j < 4410 / 2; j++) {
    //        real1 = transformedData[i][j][0];
    //        imag1 = transformedData[i][j][1];
    //        magnitude1 = sqrt(real1 * real1 + imag1 * imag1);
    //        frequency1 = j * 44100 / 4410;
    //        std::cout << "Chunk " << i << " Bin " << j << ": Frequency1 = " << frequency1 << " Hz, Magnitude1 = " << magnitude1 << std::endl;
    //        
    //        real2 = transformedData[i + (dataSize / 2)][j][0];
    //        imag2 = transformedData[i + (dataSize / 2)][j][1];
    //        magnitude2 = sqrt(real2 * real2 + imag2 * imag2);
    //        frequency2 = j * 44100 / 4410;
    //        std::cout << "Chunk " << i << " Bin " << j << ": Frequency2 = " << frequency2 << " Hz, Magnitude2 = " << magnitude2 << std::endl;
    //    }
    //}

    return RunRenderer();
}