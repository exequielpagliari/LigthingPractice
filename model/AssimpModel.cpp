
#include "AssimpModel.h"
#include "Logger.h"
#include "Shader.h"

using namespace std;

void AssimpModel::Draw(Shader &shader)
{
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool AssimpModel::loadModel(OGLRenderData& renderData, std::string modelFilename)
{
  Assimp::Importer import;

  const aiScene *scene = import.ReadFile(modelFilename, aiProcess_Triangulate | aiProcess_FlipUVs);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
      Logger::log(1, "%s Load AssimpModel Error: %s \n", __FUNCTION__, import.GetErrorString());

    return false;
  }
  directory = modelFilename.substr(0, modelFilename.find_last_of('/'));

  processMesh(scene);
    return true;
}



void AssimpModel::processMesh(const aiScene *scene)
    {
    aiMesh* mesh = scene->mMeshes[0]; // Ejemplo: primera malla

    for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
        Vertex vertex{};
        vertex.position = { mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z };
        vertex.normal   = mesh->HasNormals() ?
                          glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z) :
                          glm::vec3(0.0f);

        if (mesh->HasTextureCoords(0)) {
            vertex.texCoords = { mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y };
        } else {
            vertex.texCoords = { 0.0f, 0.0f };
        }

        vertices.push_back(vertex);
    }

    for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
        aiFace face = mesh->mFaces[f];
        for (unsigned int i = 0; i < face.mNumIndices; i++) {
            indices.push_back(face.mIndices[i]);
        }
    }
    }

void AssimpModel::createVertexBuffers() {

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Layout: posición (0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Layout: normal (1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Layout: texCoords (2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);

}
