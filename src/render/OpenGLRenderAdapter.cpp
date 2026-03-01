#include "render/OpenGLRenderAdapter.h"
#include "core/Logger.h"

OpenGLRenderAdapter::~OpenGLRenderAdapter() {
	if (window_) {
		glfwDestroyWindow(window_);
	}
}

bool OpenGLRenderAdapter::init(int width, int height, const std::string& title) {
	if (!glfwInit()) {
		LOG_ERROR("OpenGLRenderAdapter: Failed to initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!window_) {
		LOG_ERROR("OpenGLRenderAdapter: Failed to create GLFW window");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("OpenGLRenderAdapter: Failed to initialize GLAD");
		glfwDestroyWindow(window_);
		glfwTerminate();
		return false;
	}

	LOG_INFO("OpenGLRenderAdapter: Successfully initialized OpenGL context");
	return true;
}

bool OpenGLRenderAdapter::isRunning() const {
	return !glfwWindowShouldClose(window_);
}

void OpenGLRenderAdapter::pollEvents() {
	glfwPollEvents();
}

void OpenGLRenderAdapter::beginFrame(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAdapter::endFrame() {
	glfwSwapBuffers(window_);
}

void OpenGLRenderAdapter::shutdown() {
	if (window_) {
		glfwDestroyWindow(window_);
		window_ = nullptr;
	}
	glfwTerminate();
	LOG_INFO("OpenGLRenderAdapter: Shutdown complete");
}