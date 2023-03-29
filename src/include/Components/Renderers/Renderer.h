#ifndef OPENGLGP_RENDERER_H
#define OPENGLGP_RENDERER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <map>

#include "Components/Component.h"

class Model;

struct Material {
    glm::vec3 color;
    float shininess;
    // Values: <0, 1>
    float reflection;
    // Values: <0, 1>
    float refraction;
};

class Renderer : public Component {
private:
    static std::map<uint32_t, std::shared_ptr<Model>> models;
    std::shared_ptr<Model> model;

public:
    Material material;

public:
    Renderer(const std::shared_ptr<GameObject> &parent, int id);
    ~Renderer() override;


    void Update() override;

    void LoadModel(std::string path);

private:
    void Draw();
};


#endif //OPENGLGP_RENDERER_H
