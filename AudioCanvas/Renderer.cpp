#include "Renderer.h"

#include "engine_constants.h"

Renderer::Renderer() {
	mWindow = nullptr;
}

Renderer::~Renderer() {
	glfwTerminate();
}

void Renderer::Start() {
	Init();
	Run();
}

void Renderer::Init() {
	try {
		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW.");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, AUDIOCANVAS_OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, AUDIOCANVAS_OPENGL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		mWindow = glfwCreateWindow(AUDIOCANVAS_WINDOW_WIDTH, AUDIOCANVAS_WINDOW_HEIGHT, AUDIOCANVAS_WINDOW_NAME, nullptr, nullptr);
		if (mWindow == nullptr)
			throw std::runtime_error("Failed to create GLFW Window.");

		glfwMakeContextCurrent(mWindow);
		glfwSetFramebufferSizeCallback(mWindow, onFramebufferSizeCallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("Failed to initialize GLAD.");

		glViewport(0, 0, AUDIOCANVAS_WINDOW_WIDTH, AUDIOCANVAS_WINDOW_HEIGHT);

		Shader testShader("TestShader.vert");

	} catch (const std::runtime_error e) {
		std::cout << "AudioCanvas Renderer Initialization Error: " << e.what() << std::endl;
	}
}

void Renderer::Run() {
	try {
		while (!glfwWindowShouldClose(mWindow)) {
			Input();
			Update();
			Render();

			glfwSwapBuffers(mWindow);
			glfwPollEvents();
		}
	} catch (const std::runtime_error e) {
		std::cout << "AudioCanvas Renderer Runtime Error: " << e.what() << std::endl;
	}
}

void Renderer::Input() {
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow, true);
}

void Renderer::Update() {
}

void Renderer::Render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::onFramebufferSizeCallback(GLFWwindow* mWindow, int width, int height) {
	glViewport(0, 0, width, height);
}
