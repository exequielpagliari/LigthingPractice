#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "OGLRenderData.h"


class VertexBuffer {
public:
	void init();
	void uploadData(OGLMesh vertexData);
	void bind();
	void unbind();
	void draw(GLuint mode, unsigned int start,
		unsigned int num);
	void cleanup();

private:
	GLuint mVAO = 0;
	GLuint mVertexVBO = 0;
};