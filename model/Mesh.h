//
// Created by exepa on 9/15/25.
//

#pragma once

#include <glad/glad.h>
#include "OGLRenderData.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "Shader.h"

#ifndef MAIN_MESH_H
#define MAIN_MESH_H

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct TextureAssimp {
    unsigned int id;
    string type;
};
/*
class Mesh {
public:
    // mesh data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader &shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

*/
#endif //MAIN_MESH_H