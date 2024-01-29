#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <stdio.h>

#include "Renderer.h"

int main() {
	ma_result result;
	ma_engine engine;

	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		return -1;
	}

	ma_engine_play_sound(&engine, "song-show-me-how.wav", NULL);

	ma_engine_set_volume(&engine, 0.1f);

	{
		Renderer gRenderer;

		gRenderer.Start();
	}
	

	printf("Press Enter to quit...");
	getchar();

	ma_engine_uninit(&engine);

	return 0;
}