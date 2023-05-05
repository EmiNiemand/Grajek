#ifndef GLOOMENGINE_IMAGE_H
#define GLOOMENGINE_IMAGE_H

#include "Components/UI/UIComponent.h"
#include "LowLevelClasses/Shader.h"
#include "LowLevelClasses/Mesh.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <memory>

class Image : public UIComponent {
private:
    unsigned int textureID{};
    std::shared_ptr<Mesh> mesh;
    int x = 0, y = 0, width = 1920, height = 1080;
    float z = 0.0f;
    glm::vec2 leftBottom{}, leftTop{}, rightBottom{}, rightTop{};
    glm::vec3 color = glm::vec3(1.0f);
    float alpha = 1.0f;

public:
    Image(const std::shared_ptr<GameObject> &parent, int id);

    static std::shared_ptr<Mesh> CreateMesh(int x, int y, int width, int height, float z = 0);
    /**
    * x from 0 to 1920\n
    * y from 0 to 1080
    */
    void LoadTexture(int x, int y, const std::string& path, float z = 0);
    glm::vec3 GetColor();
    float GetAlpha();
    void SetPosition(float x2, float y2);
    void SetRotation(float angle);
    void SetScale(float scale);
    void SetColor(glm::vec3 newColor);
    void SetAlpha(float newAlpha);

    void Update() override;

    void Draw() override;
};

#endif //GLOOMENGINE_IMAGE_H
