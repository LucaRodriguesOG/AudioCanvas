#include <iostream>

#include "Renderer.h"
#include "Shader.h"

int main() {
	{
		Renderer gRenderer;

		gRenderer.Start();
	}

	std::cin.get();

	return 0;
}