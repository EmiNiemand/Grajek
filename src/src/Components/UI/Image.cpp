#include <iostream>
#include "Components/UI/Image.h"
#include "LowLevelClasses/Shader.h"
#include "LowLevelClasses/Mesh.h"
#include "stb_image.h"
#include "EngineManagers/UIManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/RendererManager.h"
#include "Components/Renderers/Camera.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

#define BASE_PATH "res/textures/"

Image::Image(const std::shared_ptr<GameObject> &parent, int id) : UIComponent(parent, id) {}

std::shared_ptr<Mesh> Image::CreateMesh() {
    //UpdateCorners();

    std::vector<Vertex> vertices;
    Vertex vertex1{}, vertex2{}, vertex3{}, vertex4{};

    // left bottom
    vertex1.position = glm::vec3(leftBottom.x/960-1, leftBottom.y/540-1, z);
    vertex1.texCoords = glm::vec2(0.0f, 0.0f);
    // left top
    vertex2.position = glm::vec3(leftTop.x/960-1, leftTop.y/540-1, z);
    vertex2.texCoords = glm::vec2(0.0f, 1.0f);
    // right bottom
    vertex3.position = glm::vec3(rightBottom.x/960-1, rightBottom.y/540-1, z);
    vertex3.texCoords = glm::vec2(1.0f, 0.0f);
    // right top
    vertex4.position = glm::vec3(rightTop.x/960-1, rightTop.y/540-1, z);
    vertex4.texCoords = glm::vec2(1.0f, 1.0f);

    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    vertices.push_back(vertex3);
    vertices.push_back(vertex4);

    return std::make_shared<Mesh>(vertices, std::vector<unsigned int>{0, 1, 3, 0, 2, 3}, std::vector<Texture>{});
}

void Image::LoadTexture(int x2, int y2, std::string path, float z2) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int nrChannels;
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

        filePath = path;
        x = x2; y = y2; z = z2;
        UpdateCorners();
        this->mesh = CreateMesh();
        parent->transform->SetLocalPosition(glm::vec3(x, y, z));
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    UIManager::GetInstance()->shader->Activate();
    UIManager::GetInstance()->shader->SetInt("texture1", 0);
}

void Image::SetPosition(float x2, float y2) {
    x=x2; y=y2;
    parent->transform->SetLocalPosition(glm::vec3(x, y, z));

    UpdateCorners();

    mesh->vertices[0].position = glm::vec3(leftBottom.x/960-1, leftBottom.y/540-1, z);
    mesh->vertices[1].position = glm::vec3(leftTop.x/960-1, leftTop.y/540-1, z);
    mesh->vertices[2].position = glm::vec3(rightBottom.x/960-1, rightBottom.y/540-1, z);
    mesh->vertices[3].position = glm::vec3(rightTop.x/960-1, rightTop.y/540-1, z);
    mesh->setupMesh();
}

//TODO: I actually have no idea what'd happen after using this method, probably best to avoid using it
void Image::SetRotation(float angle) {
    parent->transform->SetLocalRotation(glm::vec3(0.0f, 0.0f, angle));
    //TODO: replace these with pivot point (and add pivot point)
    float x2 = leftBottom.x, y2 = leftBottom.y;
    float width2 = rightBottom.x - x2, height2 = leftTop.y - y2;
    float p = x2 + width2 / 2, q = y2 + height2 / 2;
    float radians = glm::radians(angle);
    float cos = cosf(radians), sin = sinf(radians);
    mesh->vertices[0].position = glm::vec3(((x2 - p) * cos - (y2 - q) * sin + p)/960-1, ((x2 - p) * sin + (y2 - q) * cos + q)/540-1, z);
    mesh->vertices[1].position = glm::vec3(((x2 - p) * cos - (y2 + height2 - q) * sin + p)/960-1, ((x2 - p) * sin + (y2 + height2 - q) * cos + q)/540-1, z);
    mesh->vertices[2].position = glm::vec3(((x2 + width2 - p) * cos - (y2 - q) * sin + p)/960-1, ((x2 + width2 - p) * sin + (y2 - q) * cos + q)/540-1, z);
    mesh->vertices[3].position = glm::vec3(((x2 + width2 - p) * cos - (y2 + height2 - q) * sin + p)/960-1, ((x2 + width2 - p) * sin + (y2 + height2 - q) * cos + q)/540-1, z);
    mesh->setupMesh();
}

void Image::SetScale(float newScale) {
    scale = {newScale, newScale};
    parent->transform->SetLocalScale(glm::vec3(scale.x, scale.y, 1));

    UpdateCorners();

    mesh.reset();
    mesh = CreateMesh();
}

void Image::SetScale(glm::vec2 newScale) {
    scale = newScale;
    parent->transform->SetLocalScale(glm::vec3(scale.x, scale.y, 1));

    UpdateCorners();

    mesh->vertices[0].position = glm::vec3(leftBottom.x/960-1, leftBottom.y/540-1, z);
    mesh->vertices[1].position = glm::vec3(leftTop.x/960-1, leftTop.y/540-1, z);
    mesh->vertices[2].position = glm::vec3(rightBottom.x/960-1, rightBottom.y/540-1, z);
    mesh->vertices[3].position = glm::vec3(rightTop.x/960-1, rightTop.y/540-1, z);
    mesh->setupMesh();
}

void Image::SetColor(glm::vec3 newColor) {
    color = newColor;
    if (color.x < 0.0f) color.x = 0.0f;
    if (color.y < 0.0f) color.y = 0.0f;
    if (color.z < 0.0f) color.z = 0.0f;
    if (color.x > 1.0f) color.x = 1.0f;
    if (color.y > 1.0f) color.y = 1.0f;
    if (color.z > 1.0f) color.z = 1.0f;
}

void Image::SetAlpha(float newAlpha) {
    alpha = newAlpha;
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;
}

void Image::SetZ(float newZ) {
    z = newZ;
    parent->transform->SetLocalPosition(glm::vec3(x, y, z));

    mesh->vertices[0].position = glm::vec3(leftBottom.x/960-1, leftBottom.y/540-1, z);
    mesh->vertices[1].position = glm::vec3(leftTop.x/960-1, leftTop.y/540-1, z);
    mesh->vertices[2].position = glm::vec3(rightBottom.x/960-1, rightBottom.y/540-1, z);
    mesh->vertices[3].position = glm::vec3(rightTop.x/960-1, rightTop.y/540-1, z);
    mesh->setupMesh();
}

void Image::Update() {
#ifdef DEBUG
    ZoneScopedNC("Image", 0x800080);
#endif
    if (!mesh) return;
    if (alpha <= 0.1f) return;
    if (isDynamic) {
        glm::vec4 newPosition = glm::vec4(parent->parent->transform->GetGlobalPosition(), 1.0f);
        glm::mat4 viewProjection = RendererManager::GetInstance()->projection * Camera::activeCamera->GetComponent<Camera>()->GetViewMatrix();
        glm::vec4 postProjectivePosition = viewProjection * newPosition;

        float clipSpaceX = postProjectivePosition.x / postProjectivePosition.w;
        float clipSpaceY = postProjectivePosition.y / postProjectivePosition.w;

        float imageWidth = glm::length(((leftBottom + leftTop) - (rightBottom + rightTop)) * 0.5f);
        float imageHeight = glm::length(((leftBottom + rightBottom) - (leftTop + rightTop)) * 0.5f);

        SetPosition((clipSpaceX + 1) * 960 - imageWidth / 2, (clipSpaceY + 1) * 540 - imageHeight / 2);
    }
    UIComponent::Update();
}

void Image::OnDestroy() {
    glDeleteTextures(1, &textureID);
    mesh.reset();
    Component::OnDestroy();
}

void Image::Draw() {
    if (!mesh) return;
    UIManager::GetInstance()->shader->Activate();
    UIManager::GetInstance()->shader->SetBool("isText", false);
    UIManager::GetInstance()->shader->SetVec3("color", color);
    UIManager::GetInstance()->shader->SetFloat("alpha", alpha);
    glActiveTexture(GL_TEXTURE0);
    UIManager::GetInstance()->shader->SetInt("texture1", 0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Image::UpdateCorners() {
    // Calculate two opposite vertices
    leftBottom = {x - width*pivot.x*scale.x, y - height*pivot.y*scale.y};
    rightTop = {x + width*(1-pivot.x)*scale.x, y + height*(1-pivot.y)*scale.y};

    leftTop = {leftBottom.x, rightTop.y};
    rightBottom = {rightTop.x, leftBottom.y};
}

glm::vec3 Image::GetColor() { return color; }
float Image::GetAlpha() { return alpha; }
glm::vec2 Image::GetScale() { return scale; }

float Image::GetWidth() { return width*scale.x; }
float Image::GetHeight() { return height*scale.y; }