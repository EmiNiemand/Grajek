#include <iostream>
#include "Components/UI/Image.h"
#include "LowLevelClasses/Mesh.h"
#include "stb_image.h"
#include "EngineManagers/UIManager.h"

#define BASE_PATH "res/textures/"

Image::Image(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void Image::CreateMesh(float x, float y, float width, float height) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = {0, 1, 3,
                                         0, 2, 3};
    std::vector<Texture> textures;

    Vertex vertex1, vertex2, vertex3, vertex4;
    // left bottom
    vertex1.position = glm::vec3(2*x/1920-1, 2*y/1080-1, 0.0f);
    vertex1.texCoords = glm::vec2(0.0f, 0.0f);
    // left top
    vertex2.position = glm::vec3(2*x/1920-1, 2*y/1080-1 + height/540, 0.0f);
    vertex2.texCoords = glm::vec2(0.0f, 1.0f);
    // right bottom
    vertex3.position = glm::vec3(2*x/1920-1 + width/960, 2*y/1080-1, 0.0f);
    vertex3.texCoords = glm::vec2(1.0f, 0.0f);
    // right top
    vertex4.position = glm::vec3(2*x/1920-1 + width/960, 2*y/1080-1 + height/540, 0.0f);
    vertex4.texCoords = glm::vec2(1.0f, 1.0f);

    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    vertices.push_back(vertex3);
    vertices.push_back(vertex4);

    mesh = std::make_shared<Mesh>(vertices, indices, textures);
}

void Image::LoadTextures(float x, float y, const std::string &path) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    std::string file = BASE_PATH + path;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        this->CreateMesh(x, y, width, height);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    UIManager::GetInstance()->shader->Activate();
    UIManager::GetInstance()->shader->SetInt("texture1", 0);
}

void Image::Update() {
    Draw();
    Component::Update();
}

void Image::Draw() {
    UIManager::GetInstance()->shader->Activate();
    glActiveTexture(GL_TEXTURE0);
    UIManager::GetInstance()->shader->SetInt("texture1", 0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}