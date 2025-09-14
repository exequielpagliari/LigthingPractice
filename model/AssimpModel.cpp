#include "AssimpModel.h"
#include "Logger.h"

void AssimpModel::createVertexBuffers() {
  const tinygltf::Primitive &primitives = mModel->meshes.at(0).primitives.at(0);
  mVertexVBO.resize(primitives.attributes.size());
  for (const auto &attrib : primitives.attributes) {
    const std::string attribType = attrib.first;
    const int accessorNum = attrib.second;
    const tinygltf::Accessor &accessor = mModel->accessors.at(accessorNum);
    const tinygltf::BufferView &bufferView =
        mModel->bufferViews[accessor.bufferView];
    const tinygltf::Buffer &buffer = mModel->buffers[bufferView.buffer];
    if ((attribType.compare("POSITION") != 0) &&
        (attribType.compare("NORMAL") != 0) &&
        (attribType.compare("TEXCOORD_0") != 0)) {
      continue;
    }
    int dataSize = 1;
    switch (accessor.type) {
    case TINYGLTF_TYPE_SCALAR:
      dataSize = 1;
      break;
    case TINYGLTF_TYPE_VEC2:
      dataSize = 2;
      break;
    case TINYGLTF_TYPE_VEC3:
      dataSize = 3;
      break;
    case TINYGLTF_TYPE_VEC4:
      dataSize = 4;
      break;
    default:
      Logger::log(1, "%s error: accessor %i uses data size % i\n", __FUNCTION__,
                  accessorNum, dataSize);
      break;
    }
    GLuint dataType = GL_FLOAT;
    switch (accessor.componentType) {
    case TINYGLTF_COMPONENT_TYPE_FLOAT:
      dataType = GL_FLOAT;
      break;
    default:
      Logger::log(1, "%s error: accessor %i uses unknown data type % i\n",
                  __FUNCTION__, accessorNum, dataType);
      break;
    }
    glGenBuffers(1, &mVertexVBO[attributes[attribType]]);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO[attributes[attribType]]);
    glVertexAttribPointer(attributes[attribType], dataSize, dataType, GL_FALSE,
                          0, (void *)0);
    glEnableVertexAttribArray(attributes[attribType]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

void GltfModel::createIndexBuffer() {
  glGenBuffers(1, &mIndexVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexVBO);
}

void GltfModel::uploadVertexBuffers() {
  for (int i = 0; i < 3; ++i) {
    const tinygltf::Accessor &accessor = mModel->accessors.at(i);
    const tinygltf::BufferView &bufferView =
        mModel->bufferViews[accessor.bufferView];
    const tinygltf::Buffer &buffer = mModel->buffers[bufferView.buffer];
    glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO[i]);
    glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength,
                 &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

void GltfModel::uploadIndexBuffer() {
  const tinygltf::Primitive &primitives = mModel->meshes.at(0).primitives.at(0);
  const tinygltf::Accessor &indexAccessor =
      mModel->accessors.at(primitives.indices);
  const tinygltf::BufferView &indexBufferView =
      mModel->bufferViews[indexAccessor.bufferView];
  const tinygltf::Buffer &indexBuffer = mModel->buffers[indexBufferView.buffer];
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferView.byteLength,
               &indexBuffer.data.at(0) + indexBufferView.byteOffset,
               GL_STATIC_DRAW);
}

int GltfModel::getTriangleCount() {
  const tinygltf::Primitive &primitives = mModel->meshes.at(0).primitives.at(0);
  const tinygltf::Accessor &indexAccessor =
      mModel->accessors.at(primitives.indices);
  return indexAccessor.count;
}

bool GltfModel::loadModel(OGLRenderData &renderData,
                          std::string modelFilename) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      modelFilename, aiProcess_Triangulate | aiProcess_FlipUVs);)                                      &loaderWarnings, modelFilename);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
    return false;
  }
  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);

  glGenVertexArrays(1, &mVAO);
  glBindVertexArray(mVAO);
  createVertexBuffers();
  createIndexBuffer();
  glBindVertexArray(0);
  renderData.rdTriangleCount = getTriangleCount();
  return true;
}

void GltfModel::cleanup() {
  glDeleteBuffers(mVertexVBO.size(), mVertexVBO.data());
  glDeleteBuffers(1, &mVAO);
  glDeleteBuffers(1, &mIndexVBO);
  mTex.cleanup();
}

void GltfModel::draw() {
  const tinygltf::Primitive &primitives = mModel->meshes.at(0).primitives.at(0);
  const tinygltf::Accessor &indexAccessor =
      mModel->accessors.at(primitives.indices);
  GLuint drawMode = GL_TRIANGLES;
  switch (primitives.mode) {
  case TINYGLTF_MODE_TRIANGLES:
    drawMode = GL_TRIANGLES;
    break;
  default:
    Logger::log(1, "%s error: unknown draw mode %i\n", __FUNCTION__, drawMode);
    break;
  }

  mTex.bind();
  glBindVertexArray(mVAO);
  glDrawElements(drawMode, indexAccessor.count, indexAccessor.componentType,
                 nullptr);
  glBindVertexArray(0);
  mTex.unbind();
}
