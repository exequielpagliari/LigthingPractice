#pragma once
#include "OGLRenderData.h"
#include <glm/glm.hpp>

class Camera {
public:
  glm::mat4 getViewMatrix(OGLRenderData &renderData);
  glm::vec3 mViewDirection = glm::vec3(0.0f, 0.0f, 0.0f);

private:
  glm::vec3 mWorldPos = glm::vec3(0.5f, 0.25f, 1.0f);
  glm::vec3 mWorldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 mRightDirection = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 mUpDirection = glm::vec3(0.0f, 0.0f, 0.0f);
};
