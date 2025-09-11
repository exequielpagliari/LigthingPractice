#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Framebuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "OGLRenderData.h"
#include "UniformBuffer.h"
#include "UserInterface.h"
#include "Timer.h"
#include "Camera.h"
#include "GltfModel.h"


class OGLRenderer {
public:

	OGLRenderer(GLFWwindow *window);
	bool init(unsigned int width, unsigned int height);
	void setSize(unsigned int width, unsigned int height);
	void cleanup();
	void uploadData(OGLMesh vertexData);
	void draw();
	void handleKeyEvents(int key, int scancode, int action, int mods);
	void handleMouseButtonEvents(int button, int action, int mods);
	void handleMousePositionEvents(double xPos, double yPos);
private:
	Shader mShader{};
	Shader mChangedShader{};
	Shader mGltfShader{};
	std::shared_ptr<GltfModel> mGltfModel = nullptr;

	Framebuffer mFramebuffer{};
	VertexBuffer mVertexBuffer{};
	Texture mTex{};
	UniformBuffer mUniformBuffer{};
	OGLRenderData mRenderData{};
	UserInterface mUserInterface{};
	Timer mUIGenerateTimer{};
	Timer mUIDrawTimer{};
	Timer mMatrixUploadTimer{};
	Timer mMatrixGenerateTimer;

	glm::mat4 mViewMatrix = glm::mat4(1.0f);
	glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
	Camera mCamera{};
	bool mMouseLock = false;
	int mMouseXPos = 0;
	int mMouseYPos = 0;
	double lastTickTime = 0.0;

	void handleMovementKeys();
};