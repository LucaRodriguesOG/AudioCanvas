#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Shader.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void Start();
private:
	void Init();
	void Run();

	void Input();
	void Update(double deltaTime);
	void Render();

	static void onFramebufferSizeCallback(GLFWwindow*, int, int);

	GLFWwindow* mWindow;
	Shader mShader;
	GLuint mVertexArrayObject;
};