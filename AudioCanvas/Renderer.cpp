#include "Renderer.h"

#include "engine_constants.h"

Renderer::Renderer() {
	mWindow = nullptr;
	mVertexArrayObject = GL_ZERO;
}

Renderer::~Renderer() {
	glfwTerminate();
}

void Renderer::Start() {
	Init();
	Run();
}

void Renderer::SetChannelData(std::vector<fftw_complex*>* channel01, std::vector<fftw_complex*>* channel02) {
	mChannel01Data = channel01;
	mChannel02Data = channel02;
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
		glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("Failed to initialize GLAD.");

		glViewport(0, 0, AUDIOCANVAS_WINDOW_WIDTH, AUDIOCANVAS_WINDOW_HEIGHT);

		mShader.AddShaders({ "TestShader.vert", "TestShader.frag" });

		mShader.Compile();

		glGenVertexArrays(1, &mVertexArrayObject);

		glBindVertexArray(mVertexArrayObject);

		glUseProgram(mShader.GetProgramID());
	} catch (const std::runtime_error e) {
		std::cout << "AudioCanvas Renderer Initialization Error: " << e.what() << std::endl;
		std::cout << "<!-- END OF ERROR --!>" << std::endl;
	}
}

void Renderer::Run() {
	try {
		double deltaTime = 0.0, lastTime = 0.0, now = 0.0, secondCounter = glfwGetTime();
		long long framesPerSecond = 0;
		while (!glfwWindowShouldClose(mWindow)) {
			now = glfwGetTime();
			deltaTime = now - lastTime;
			lastTime = now;

			Input();
			Update(deltaTime);
			Render();

			glfwSwapBuffers(mWindow);
			glfwPollEvents();

			if (now - secondCounter < 1.0) {
				framesPerSecond++;
			} else {
				std::string title = "AudioCanvas v0.5 FPS: " + std::to_string(framesPerSecond);
				glfwSetWindowTitle(mWindow, title.c_str());
				framesPerSecond = 0;
				secondCounter = glfwGetTime();
			}
		}
	} catch (const std::runtime_error e) {
		std::cout << "AudioCanvas Renderer Runtime Error: " << e.what() << std::endl;
		std::cout << "<!-- END OF ERROR --!>" << std::endl;
	}
}

void Renderer::Input() {
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow, true);
}

void Renderer::Update(double deltaTime) {
	glUniform1f(glGetUniformLocation(mShader.GetProgramID(), "deltaTime"), (float)deltaTime);
	glUniform1f(glGetUniformLocation(mShader.GetProgramID(), "programTime"), (float)glfwGetTime());
	glUniform2i(glGetUniformLocation(mShader.GetProgramID(), "canvasResolution"), AUDIOCANVAS_WINDOW_WIDTH, AUDIOCANVAS_WINDOW_HEIGHT);
}

void Renderer::Render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLuint Renderer::CreateChunkTexture(fftw_complex* chunk) {
	GLuint tempTextureID = 0;
	glGenTextures(1, &tempTextureID);
	glBindTexture(GL_TEXTURE_1D, tempTextureID);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, 4410, 0, GL_RED, GL_DOUBLE, chunk);

	return tempTextureID;
}

void Renderer::UpdateChunkTexture(fftw_complex*, int) {
}

void Renderer::onFramebufferSizeCallback(GLFWwindow* mWindow, int width, int height) {
	glViewport(0, 0, width, height);
}