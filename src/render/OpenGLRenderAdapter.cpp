#include "render/OpenGLRenderAdapter.h"
#include "core/Logger.h"

namespace {
void framebufferSizeCallback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}
}

OpenGLRenderAdapter::~OpenGLRenderAdapter() {
	shutdown();
}

bool OpenGLRenderAdapter::init(int width, int height, const std::string& title) {
	shutdown();

	if (!glfwInit()) {
		LOG_ERROR("OpenGLRenderAdapter: Failed to initialize GLFW");
		return false;
	}
	initialized_ = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!window_) {
		LOG_ERROR("OpenGLRenderAdapter: Failed to create GLFW window");
		shutdown();
		return false;
	}

	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("OpenGLRenderAdapter: Failed to initialize GLAD");
		shutdown();
		return false;
	}

	glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
	glViewport(0, 0, width, height);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	if (!createRenderResources()) {
		LOG_ERROR("OpenGLRenderAdapter: Failed to create render resources");
		shutdown();
		return false;
	}

	LOG_INFO("OpenGLRenderAdapter: Successfully initialized OpenGL context");
	return true;
}

bool OpenGLRenderAdapter::isRunning() const {
	return window_ != nullptr && !glfwWindowShouldClose(window_);
}

void OpenGLRenderAdapter::pollEvents() {
	if (initialized_) {
		glfwPollEvents();
	}
}

void OpenGLRenderAdapter::beginFrame(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAdapter::drawPrimitive(PrimitiveType primitive, const Mat4& modelMatrix, const Vec4& color) {
	const PrimitiveMesh* mesh = getMesh(primitive);
	if (mesh == nullptr || shader_.getId() == 0) {
		return;
	}

	shader_.use();
	glUniformMatrix4fv(modelLocation_, 1, GL_FALSE, modelMatrix.data());
	glUniform4f(colorLocation_, color.x, color.y, color.z, color.w);

	glBindVertexArray(mesh->vao);
	glDrawArrays(mesh->drawMode, 0, mesh->vertexCount);
	glBindVertexArray(0);
}

void OpenGLRenderAdapter::endFrame() {
	if (window_) {
		glfwSwapBuffers(window_);
	}
}

void OpenGLRenderAdapter::shutdown() {
	bool released = false;

	destroyRenderResources();

	if (window_) {
		glfwDestroyWindow(window_);
		window_ = nullptr;
		released = true;
	}
	if (initialized_) {
		glfwTerminate();
		initialized_ = false;
		released = true;
	}

	if (released) {
		LOG_INFO("OpenGLRenderAdapter: Shutdown complete");
	}
}

bool OpenGLRenderAdapter::createRenderResources() {
	if (!shader_.load("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl")) {
		return false;
	}

	modelLocation_ = glGetUniformLocation(shader_.getId(), "uModel");
	colorLocation_ = glGetUniformLocation(shader_.getId(), "uColor");

	if (modelLocation_ < 0 || colorLocation_ < 0) {
		LOG_ERROR("OpenGLRenderAdapter: Failed to find shader uniforms");
		return false;
	}

	static const float lineVertices[] = {
		-0.5f, 0.0f, 0.0f,
		 0.5f, 0.0f, 0.0f
	};

	static const float triangleVertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	static const float quadVertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	static const float cubeVertices[] = {
		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,

		 0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f
	};

	if (!setupMesh(lineMesh_, lineVertices, 2, GL_LINES)) {
		return false;
	}

	if (!setupMesh(triangleMesh_, triangleVertices, 3, GL_TRIANGLES)) {
		return false;
	}

	if (!setupMesh(quadMesh_, quadVertices, 6, GL_TRIANGLES)) {
		return false;
	}

	if (!setupMesh(cubeMesh_, cubeVertices, 36, GL_TRIANGLES)) {
		return false;
	}

	return true;
}

void OpenGLRenderAdapter::destroyRenderResources() {
	auto destroyMesh = [](PrimitiveMesh& mesh) {
		if (mesh.vbo != 0) {
			glDeleteBuffers(1, &mesh.vbo);
			mesh.vbo = 0;
		}
		if (mesh.vao != 0) {
			glDeleteVertexArrays(1, &mesh.vao);
			mesh.vao = 0;
		}
		mesh.vertexCount = 0;
		mesh.drawMode = GL_TRIANGLES;
	};

	destroyMesh(lineMesh_);
	destroyMesh(triangleMesh_);
	destroyMesh(quadMesh_);
	destroyMesh(cubeMesh_);
	modelLocation_ = -1;
	colorLocation_ = -1;
	shader_.destroy();
}

bool OpenGLRenderAdapter::setupMesh(PrimitiveMesh& mesh, const float* vertices, GLsizei vertexCount, GLenum drawMode) {
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);

	if (mesh.vao == 0 || mesh.vbo == 0) {
		return false;
	}

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexCount * 3 * sizeof(float)), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	mesh.vertexCount = vertexCount;
	mesh.drawMode = drawMode;
	return true;
}

const OpenGLRenderAdapter::PrimitiveMesh* OpenGLRenderAdapter::getMesh(PrimitiveType primitive) const {
	switch (primitive) {
	case PrimitiveType::Line:
		return &lineMesh_;
	case PrimitiveType::Triangle:
		return &triangleMesh_;
	case PrimitiveType::Quad:
		return &quadMesh_;
	case PrimitiveType::Cube:
		return &cubeMesh_;
	default:
		return nullptr;
	}
}
