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
    int x, y, width, height;

public:
    Image(const std::shared_ptr<GameObject> &parent, int id);

    static std::shared_ptr<Mesh> CreateMesh(int x, int y, int width, int height);
    static std::shared_ptr<Mesh> CreateMeshFromPoints(int leftBottomX, int leftBottomY, int leftTopX, int leftTopY, int rightBottomX, int rightBottomY, int rightTopX, int rightTopY);
    /**
    * x from 0 to 1920\n
    * y from 0 to 1080
    */
    void LoadTexture(int x, int y, const std::string& path);
    void SetPosition(int newX, int newY);
    void SetRotation(int angle);
    void SetScale(float scale);

    void Update() override;

    void Draw() override;
};

#endif //GLOOMENGINE_IMAGE_H
