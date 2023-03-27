//
// Created by szymo on 30/10/2022.
//

#ifndef OPENGLGP_MESH_H
#define OPENGLGP_MESH_H

#include <string>
#include <vector>
#include <memory>

#include "Shader.h"

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoords;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 biTangent;
    //bone indexes which will influence this vertex
    glm::i16vec4 boneIDs;
    //weights from each bone
    glm::vec4 weights;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int vao;

    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // render the mesh
    void Draw(std::shared_ptr<Shader> &shader, int type);

private:
    // render data
    unsigned int vbo, ebo;

    // initializes all the buffer objects/arrays
    void setupMesh();
};


#endif //OPENGLGP_MESH_H
