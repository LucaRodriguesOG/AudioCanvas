#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fftw3.h>

#include <iostream>
#include <string>

#include "Shader.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void Start();

	void SetChannelData(std::vector<fftw_complex*>* channel01, std::vector<fftw_complex*>* channel02);
private:
	void Init();
	void Run();

	void Input();
	void Update(double deltaTime);
	void Render();

	GLuint CreateChunkTexture(fftw_complex*);
	void UpdateChunkTexture(fftw_complex*, int);

	static void onFramebufferSizeCallback(GLFWwindow*, int, int);

	GLFWwindow* mWindow;
	Shader mShader;
	GLuint mVertexArrayObject;

	GLint mChannel01Texture;
	GLint mChannel02Texture;

	std::vector<fftw_complex*>* mChannel01Data;
	std::vector<fftw_complex*>* mChannel02Data;
};