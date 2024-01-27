#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Renderer {
public:
	Renderer();
	~Renderer();

	void Start();
private:
	void Init();
	void Run();

	void Input();
	void Update();
	void Render();

	static void onFramebufferSizeCallback(GLFWwindow*, int, int);

	GLFWwindow* mWindow;
};