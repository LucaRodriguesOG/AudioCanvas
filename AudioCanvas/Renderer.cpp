#include "Renderer.h"

#include "engine_constants.h"

Renderer::Renderer(std::vector<float>* data, std::mutex* mutex) : mData(data), mDataMutex(mutex) {
	mWindow = nullptr;
	mVertexArrayObject = GL_ZERO;
  
	mAvailableShaders = {
		{ "acsvShader.vert", "FFT_Display.frag" },
		{ "acsvShader.vert", "rVisualizer.frag" },
		{ "acsvShader.vert", "Jump.frag" },
		{ "acsvShader.vert", "rVisualizer2.frag" }
	};

	mCurrentShader = 0;
	autoIter = false;

	Init();
}

Renderer::~Renderer() {
	glfwTerminate();
}

void Renderer::Start() {
	Run();
}

void Renderer::CreateRTAudioTexture() {
	glGenTextures(1, &mRTAudioTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, mRTAudioTexture);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, mData->size() / 2, 0, GL_RED, GL_FLOAT, mData->data());
}

void Renderer::SetRTAudioTexture() {
	glBindTexture(GL_TEXTURE_1D, mRTAudioTexture);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, mData->size(), 0, GL_RED, GL_FLOAT, mData->data());
	glFinish();
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
		glfwSetKeyCallback(mWindow, onKeyCallback);
		glfwSwapInterval(0);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("Failed to initialize GLAD.");

		glViewport(0, 0, AUDIOCANVAS_WINDOW_WIDTH, AUDIOCANVAS_WINDOW_HEIGHT);

		mShader.AddShaders(mAvailableShaders.at(mCurrentShader));

		mShader.Compile();

		mInputTime = glfwGetTime();

		glGenVertexArrays(1, &mVertexArrayObject);

		glBindVertexArray(mVertexArrayObject);

		glUseProgram(mShader.GetProgramID());

		CreateRTAudioTexture();
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
	if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS && (glfwGetTime() - mInputTime > 1.0)) {
		mInputTime = glfwGetTime();
		std::cout << "Issue recompile" << std::endl;
		glUseProgram(mShader.Recompile(mAvailableShaders.at(mCurrentShader)));
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS && (glfwGetTime() - mInputTime > 1.0)) {
		mInputTime = glfwGetTime();
		std::cout << "Loading Previous Shader" << std::endl;
		if (mCurrentShader == 0) {
			mCurrentShader = mAvailableShaders.size() - 1;
		} else {
			mCurrentShader--;
		}
		glUseProgram(mShader.Recompile(mAvailableShaders.at(mCurrentShader)));
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS && (glfwGetTime() - mInputTime > 1.0)) {
		mInputTime = glfwGetTime();
		std::cout << "Loading Next Shader" << std::endl;
		if (mCurrentShader == mAvailableShaders.size() - 1) {
			mCurrentShader = 0;
		} else {
			mCurrentShader++;
		}
		glUseProgram(mShader.Recompile(mAvailableShaders.at(mCurrentShader)));
	}
	if (glfwGetKey(mWindow, GLFW_KEY_ENTER) == GLFW_PRESS && (glfwGetTime() - mInputTime > 1.0)) {
		mInputTime = glfwGetTime();
		std::cout << "Auto-Play Toggled: " << !autoIter << std::endl;
		autoIter = !autoIter;
	}
}

void Renderer::Update(double deltaTime) {
	glUniform1f(glGetUniformLocation(mShader.GetProgramID(), "deltaTime"), (float)deltaTime);
	glUniform1f(glGetUniformLocation(mShader.GetProgramID(), "programTime"), (float)glfwGetTime());
	glUniform2i(glGetUniformLocation(mShader.GetProgramID(), "canvasResolution"), AUDIOCANVAS_WINDOW_WIDTH, AUDIOCANVAS_WINDOW_HEIGHT);
	std::lock_guard<std::mutex> lock(*mDataMutex);
	if (glfwGetTime() - mUpdateTime > 0.03) {
		SetRTAudioTexture();
		mUpdateTime = glfwGetTime();
	}
	if (glfwGetTime() - autoPlayTime > 10.0 && autoIter) {
		autoPlayTime = glfwGetTime();
		if (mCurrentShader == mAvailableShaders.size() - 1) {
			mCurrentShader = 0;
		} else {
			mCurrentShader++;
		}
		glUseProgram(mShader.Recompile(mAvailableShaders.at(mCurrentShader)));
	}
}

void Renderer::Render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::onFramebufferSizeCallback(GLFWwindow* mWindow, int width, int height) {
	glViewport(0, 0, width, height);
}

void Renderer::onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		std::cout << "Issue recompile." << std::endl;
}