#include <iostream>
#include "Components/UI/Image.h"
#include "LowLevelClasses/Mesh.h"
#include "stb_image.h"
#include "EngineManagers/UIManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

#define BASE_PATH "res/textures/"

Image::Image(const std::shared_ptr<GameObject> &parent, int id) : UIComponent(parent, id) {}

std::shared_ptr<Mesh> Image::CreateMesh(int x, int y, int width, int height) {
    std::vector<Vertex> vertices;
    Vertex vertex1, vertex2, vertex3, vertex4;

    // left bottom
    vertex1.position = glm::vec3((float)x/960-1, (float)y/540-1, 0.0f);
    vertex1.texCoords = glm::vec2(0.0f, 0.0f);
    // left top
    vertex2.position = glm::vec3((float)x/960-1, (float)y/540-1 + (float)height/540, 0.0f);
    vertex2.texCoords = glm::vec2(0.0f, 1.0f);
    // right bottom
    vertex3.position = glm::vec3((float)x/960-1 + (float)width/960, (float)y/540-1, 0.0f);
    vertex3.texCoords = glm::vec2(1.0f, 0.0f);
    // right top
    vertex4.position = glm::vec3((float)x/960-1 + (float)width/960, (float)y/540-1 + (float)height/540, 0.0f);
    vertex4.texCoords = glm::vec2(1.0f, 1.0f);

    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    vertices.push_back(vertex3);
    vertices.push_back(vertex4);

    return std::make_shared<Mesh>(vertices, std::vector<unsigned int>{0, 1, 3, 0, 2, 3}, std::vector<Texture>{});
}

std::shared_ptr<Mesh> Image::CreateMeshFromPoints(int leftBottomX, int leftBottomY, int leftTopX, int leftTopY, int rightBottomX, int rightBottomY, int rightTopX, int rightTopY) {
    std::vector<Vertex> vertices;
    Vertex vertex1, vertex2, vertex3, vertex4;
    vertex1.position = glm::vec3((float)leftBottomX/960-1, (float)leftBottomY/540-1, 0.0f);
    vertex1.texCoords = glm::vec2(0.0f, 0.0f);
    vertex2.position = glm::vec3((float)leftTopX/960-1, (float)leftTopY/540-1, 0.0f);
    vertex2.texCoords = glm::vec2(0.0f, 1.0f);
    vertex3.position = glm::vec3((float)rightBottomX/960-1, (float)rightBottomY/540-1, 0.0f);
    vertex3.texCoords = glm::vec2(1.0f, 0.0f);
    vertex4.position = glm::vec3((float)rightTopX/960-1, (float)rightTopY/540-1, 0.0f);
    vertex4.texCoords = glm::vec2(1.0f, 1.0f);
    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    vertices.push_back(vertex3);
    vertices.push_back(vertex4);
    return std::make_shared<Mesh>(vertices, std::vector<unsigned int>{0, 1, 3, 0, 2, 3}, std::vector<Texture>{});
}

void Image::LoadTexture(int x, int y, const std::string &path) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int width, height, nrChannels;
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

        this->mesh = CreateMesh(x, y, width, height);
        leftBottom.x = x;
        leftBottom.y = y;
        leftTop.x = x;
        leftTop.y = y + height;
        rightBottom.x = x + width;
        rightBottom.y = y;
        rightTop.x = x + width;
        rightTop.y = y + height;
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        parent->transform->SetLocalPosition(glm::vec3(x, y, 0.0f));
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    UIManager::GetInstance()->shader->Activate();
    UIManager::GetInstance()->shader->SetInt("texture1", 0);
}

void Image::SetPosition(int x2, int y2) {
    parent->transform->SetLocalPosition(glm::vec3(x2, y2, 0.0f));
    float width2 = rightBottom.x - leftBottom.x, height2 = leftTop.y - leftBottom.y;
    leftBottom.x = x2; leftBottom.y = y2;
    leftTop.x = x2; leftTop.y = y2 + height2;
    rightBottom.x = x2 + width2; rightBottom.y = y2;
    rightTop.x = x2 + width2; rightTop.y = y2 + height2;
    mesh->vertices[0].position = glm::vec3(leftBottom.x/960-1, leftBottom.y/540-1, 0.0f);
    mesh->vertices[1].position = glm::vec3(leftTop.x/960-1, leftTop.y/540-1, 0.0f);
    mesh->vertices[2].position = glm::vec3(rightBottom.x/960-1, rightBottom.y/540-1, 0.0f);
    mesh->vertices[3].position = glm::vec3(rightTop.x/960-1, rightTop.y/540-1, 0.0f);
    mesh->setupMesh();
}

void Image::SetRotation(float angle) {
    parent->transform->SetLocalRotation(glm::vec3(0.0f, 0.0f, angle));
    float x2 = leftBottom.x, y2 = leftBottom.y;
    float width2 = rightBottom.x - x2, height2 = leftTop.y - y2;
    float p = x2 + width2 / 2, q = y2 + height2 / 2;
    float radians = glm::radians(angle);
    float cos = cosf(radians), sin = sinf(radians);
    mesh->vertices[0].position = glm::vec3(((x2 - p) * cos - (y2 - q) * sin + p)/960-1, ((x2 - p) * sin + (y2 - q) * cos + q)/540-1, 0.0f);
    mesh->vertices[1].position = glm::vec3(((x2 - p) * cos - (y2 + height2 - q) * sin + p)/960-1, ((x2 - p) * sin + (y2 + height2 - q) * cos + q)/540-1, 0.0f);
    mesh->vertices[2].position = glm::vec3(((x2 + width2 - p) * cos - (y2 - q) * sin + p)/960-1, ((x2 + width2 - p) * sin + (y2 - q) * cos + q)/540-1, 0.0f);
    mesh->vertices[3].position = glm::vec3(((x2 + width2 - p) * cos - (y2 + height2 - q) * sin + p)/960-1, ((x2 + width2 - p) * sin + (y2 + height2 - q) * cos + q)/540-1, 0.0f);
    mesh->setupMesh();
}

void Image::SetScale(float scale) {
    parent->transform->SetLocalScale(glm::vec3(scale));
    leftBottom.x = x + (float)width / 2 - (float)width * scale / 2;
    leftBottom.y = y + (float)height / 2 - (float)height * scale / 2;
    leftTop.x = leftBottom.x;
    leftTop.y = leftBottom.y + (float)height * scale;
    rightBottom.x = leftBottom.x + (float)width * scale;
    rightBottom.y = leftBottom.y;
    rightTop.x = rightBottom.x;
    rightTop.y = leftTop.y;
    mesh->vertices[0].position = glm::vec3(leftBottom.x/960-1, leftBottom.y/540-1, 0.0f);
    mesh->vertices[1].position = glm::vec3(leftTop.x/960-1, leftTop.y/540-1, 0.0f);
    mesh->vertices[2].position = glm::vec3(rightBottom.x/960-1, rightBottom.y/540-1, 0.0f);
    mesh->vertices[3].position = glm::vec3(rightTop.x/960-1, rightTop.y/540-1, 0.0f);
    mesh->setupMesh();
}

void Image::Update() {
#ifdef DEBUG
    ZoneScopedNC("Image", 0x800080);
#endif
    UIComponent::Update();
}

void Image::Draw() {
    if (!mesh) return;
    UIManager::GetInstance()->shader->Activate();
    UIManager::GetInstance()->shader->SetBool("isText", false);
    glActiveTexture(GL_TEXTURE0);
    UIManager::GetInstance()->shader->SetInt("texture1", 0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}