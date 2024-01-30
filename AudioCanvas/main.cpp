#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <stdio.h>
#include <numeric>

#include "Renderer.h"

int RunRenderer() {
	{
		Renderer gRenderer;

		gRenderer.Start();
	}

	return 0;
}

int main() {
	ma_decoder decoder;

	ma_result result = ma_decoder_init_file("song-through-a-cardboard-world.wav", nullptr, &decoder);
	if (result != MA_SUCCESS) {
		return false;   // An error occurred.
	}

	ma_uint64 length;

	result = ma_decoder_get_length_in_pcm_frames(&decoder, &length);

	ma_decoder_seek_to_pcm_frame(&decoder, 0);

	ma_int16 frameData[44100];

	ma_uint64 framesRead = 0;
	result = ma_decoder_read_pcm_frames(&decoder, frameData, 44100, &framesRead);

	ma_decoder_uninit(&decoder);

	return RunRenderer();
}