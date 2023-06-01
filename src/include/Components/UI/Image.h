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
    unsigned int textureID;
    std::shared_ptr<Mesh> mesh;
    glm::vec2 leftBottom{}, leftTop{}, rightBottom{}, rightTop{};
    // Original dimensions of the image
    int width = 1920, height = 1080;
    int x = 0, y = 0;
    float z = 0.0f;
    glm::vec2 scale = glm::vec2(1);

    glm::vec3 color = glm::vec3(1.0f);
    float alpha = 1.0f;

public:
    /// Value range: (<0, 1>, <0, 1>)
    /// Describes offset from leftBottom corner
    glm::vec2 pivot = {0, 0};

    Image(const std::shared_ptr<GameObject> &parent, int id);

    std::shared_ptr<Mesh> CreateMesh();
    /**
    * x from 0 to 1920\n
    * y from 0 to 1080
    */
    void LoadTexture(int x, int y, const std::string& path, float z = 0);

    void Update() override;
    void OnDestroy() override;

    void Draw() override;

    void SetPosition(float x2, float y2) override;
    void SetRotation(float angle) override;
    void SetScale(float scale) override;
    void SetScale(glm::vec2 newScale);
    void SetColor(glm::vec3 newColor) override;
    void SetAlpha(float newAlpha) override;

    float GetWidth();
    float GetHeight();
    glm::vec2 GetScale();
    glm::vec3 GetColor() override;
    float GetAlpha() override;

private:
    void UpdateCorners();
};

#endif //GLOOMENGINE_IMAGE_H
