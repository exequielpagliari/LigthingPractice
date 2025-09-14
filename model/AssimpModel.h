#pragma once
#include "OGLRenderData.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <memory>
#include <string>
#include <tiny_gltf.h>
#include <vector>

class AssimpModel {
public:
  bool loadModel(OGLRenderData &renderData, std::string modelFilename);
  void draw();
  void cleanup();
  void uploadVertexBuffers();
  void uploadIndexBuffer();

private:
  void createVertexBuffers();
  void createIndexBuffer();
  int getTriangleCount();
  GLuint mVAO = 0;
  std::vector<GLuint> mVertexVBO{};
  GLuint mIndexVBO = 0;
  std::map<std::string, GLint> attributes = {
      {"POSITION", 0}, {"NORMAL", 1}, {"TEXCOORD_0", 2}};
  Texture mTex{};
};
