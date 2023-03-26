#ifndef OPENGLGP_RENDERER_H
#define OPENGLGP_RENDERER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"

class Model;

class Renderer : public Component {
private:
    std::shared_ptr<Model> model;
public:
    std::string path;
    glm::vec3 objectColor;
    float shininess;

public:
    Renderer(const std::shared_ptr<GameObject> &parent, int id);
    ~Renderer() override;


    void Update() override;
    void OnUpdate() override;

    void LoadModel(std::string newPath);

private:
    void Draw();
};


#endif //OPENGLGP_RENDERER_H
