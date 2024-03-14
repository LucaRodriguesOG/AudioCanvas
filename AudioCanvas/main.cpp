#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <limits>

#include "DSP.h"
#include "Renderer.h"
#include "GlobalConstants.h"

int RunRenderer() {
	{
		Renderer gRenderer;

		gRenderer.Start();
	}

	return 0;
}

int main() {
	{
		std::vector<std::vector<float>> c1Data;
		std::vector<std::vector<float>> c2Data;

		DSP dsp("song-through-a-cardboard-world.wav");
		dsp.Run(&c1Data, &c2Data);

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