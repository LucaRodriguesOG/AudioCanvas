#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <mutex>

#include "Shader.h"

class Renderer {
public:
	Renderer(std::vector<float>*, std::mutex*);
	~Renderer();

	void Start();

	void CreateRTAudioTexture();
	void SetRTAudioTexture();
private:
	void Init();
	void Run();

	void Input();
	void Update(double deltaTime);
	void Render();

	static void onFramebufferSizeCallback(GLFWwindow*, int, int);
	static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	double mInputTime;

	std::vector<std::vector<std::string>> mAvailableShaders;
	int mCurrentShader;
	bool autoIter;
	double autoPlayTime;

	GLFWwindow* mWindow;
	Shader mShader;
	GLuint mVertexArrayObject;

	GLuint mRTAudioTexture;
	std::vector<float>* mData;
	std::mutex* mDataMutex;
	double mUpdateTime;
};