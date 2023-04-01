#ifndef GLOOMENGINE_IMAGE_H
#define GLOOMENGINE_IMAGE_H

#include "Components/Component.h"
#include "LowLevelClasses/Shader.h"
#include "LowLevelClasses/Mesh.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <memory>

class Image : public Component {
private:
    unsigned int textureID;
    std::shared_ptr<Mesh> mesh;

public:
    Image(const std::shared_ptr<GameObject> &parent, int id);

    void CreateMesh(float x, float y, float width, float height);

    void LoadTextures(float x, float y, const std::string& path);

    void Update() override;

    void Draw();
};

#endif //GLOOMENGINE_IMAGE_H
