//
// Created by szymo on 01/04/2023.
//

#ifndef GLOOMENGINE_DRAWABLE_H
#define GLOOMENGINE_DRAWABLE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <unordered_map>

#include "Components/Component.h"

class Shader;

struct Material {
    glm::vec3 color;
    float shininess;
    // Values: <0, 1>
    float reflection;
    // Values: <0, 1>
    float refraction;
};

class Drawable : public Component {
public:
    Material material = {{1.0f, 1.0f, 1.0f},32.0f,0,0};
    glm::vec2 textScale = glm::vec2(1.0f, 1.0f);
public:
    Drawable(const std::shared_ptr<GameObject> &parent, uint32_t id);
    ~Drawable() override;

    void Update() override;
    virtual void Draw() = 0;
    virtual void Draw(std::shared_ptr<Shader> shader) = 0;

private:
    void AddToDraw();
};


#endif //GLOOMENGINE_DRAWABLE_H
