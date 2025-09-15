#pragma once

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "OGLRenderData.h"
#include "Texture.h"
#include "Mesh.h"

using namespace  std;


class AssimpModel {
public:
    bool loadModel(OGLRenderData& renderData, std::string modelFilename);
    void Draw(Shader &shader);
private:

    // model data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    string directory;

    GLuint mVAO = 0;
    std::vector<GLuint> mVertexVBO{};
    GLuint mVBO = 0;
    GLuint mEBO;
    GLuint mIndexVBO = 0;


    void processMesh(const aiScene *scene);
    void createVertexBuffers();

};
