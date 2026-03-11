#pragma once
#include "IRenderAdapter.h"
#include "render/ShaderProgram.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLRenderAdapter : public IRenderAdapter {
	public:
	virtual ~OpenGLRenderAdapter() override;
	virtual bool init(int width, int height, const std::string& title) override;
	virtual bool isRunning() const override;
	virtual void pollEvents() override;
	virtual void beginFrame(float r, float g, float b) override;
	virtual void drawPrimitive(PrimitiveType primitive, const Mat4& modelMatrix, const Vec4& color) override;
	virtual void endFrame() override;
	virtual void shutdown() override;

	GLFWwindow* getWindow() const { return window_; }

private:
	struct PrimitiveMesh {
		GLuint vao = 0;
		GLuint vbo = 0;
		GLsizei vertexCount = 0;
	};

	bool createRenderResources();
	void destroyRenderResources();
	bool setupMesh(PrimitiveMesh& mesh, const float* vertices, GLsizei vertexCount);
	const PrimitiveMesh* getMesh(PrimitiveType primitive) const;

	GLFWwindow* window_ = nullptr;
	bool initialized_ = false;
	ShaderProgram shader_;
	PrimitiveMesh triangleMesh_;
	PrimitiveMesh quadMesh_;
	GLint modelLocation_ = -1;
	GLint colorLocation_ = -1;
};
