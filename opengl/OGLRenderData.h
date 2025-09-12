#pragma once
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <vector>

struct OGLVertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 uv;
};
struct OGLMesh {
  std::vector<OGLVertex> vertices;
};

struct OGLRenderData {
  GLFWwindow *rdWindow = nullptr;
  int rdWidth = 0;
  int rdHeight = 0;
  unsigned int rdTriangleCount = 0;

  float rdFrameTime = 0.0f;
  float rdUIGenerateTime = 0.0f;
  float rdUIDrawTime = 0.0f;
  float rdMatrixGenerateTime = 0.0f;
  float rdMatrixUploadTime = 0.0f;

  unsigned int rdGltfTriangleCount = 0;

  bool rdUseChangedShader = false;

  int rdFieldOfView = 90;

  float rdViewAzimuth = 320.0f;
  float rdViewElevation = -15.0f;

  int rdMoveForward = 0;
  int rdMoveRight = 0;
  int rdMoveUp = 0;

  float rdTickDiff = 0.0f;
  glm::vec3 rdCameraWorldPosition = glm::vec3(0.5f, 0.25f, 1.0f);
  glm::vec3 rdCameraWorldDirection = glm::vec3(0.0f, 0.0f, 0.0f);

  float ambStr = 0.1f;
  float spcStr = 0.1f;

  float rShader = 0.0f;
  float gShader = 0.0f;
  float bShader = 0.0f;

  float xLight = 0.0f;
  float yLight = 0.0f;
  float zLight = 0.0f;
};
