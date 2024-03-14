#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fftw3.h>

#include <iostream>
#include <string>

#include "Shader.h"
#include "GlobalConstants.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void Start();

	void SetChannelData(std::vector<std::vector<float>>* channel01, std::vector<std::vector<float>>* channel02);
private:
	void Init();
	void Run();

	void Input();
	void Update(double deltaTime);
	void Render();

	GLuint CreateChunkTexture(std::vector<float>);
	void UpdateChunkTexture(std::vector<float> chunk, int);

	static void onFramebufferSizeCallback(GLFWwindow*, int, int);

	GLFWwindow* mWindow;
	Shader mShader;
	GLuint mVertexArrayObject;

	GLint mChannel01Texture;
	GLint mChannel02Texture;

	std::vector<std::vector<float>>* mChannel01Data;
	std::vector<std::vector<float>>* mChannel02Data;

	GLuint mCurrentChunkIndex;
};