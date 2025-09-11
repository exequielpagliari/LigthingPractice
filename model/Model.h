#pragma once
#include <vector>
#include "OGLRenderData.h"
#include <glm/glm.hpp>



class Model {
public:
	void init();
	OGLMesh getVertexData();
private:
	OGLMesh mVertexData{};
};