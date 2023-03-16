//
// Created by szymo on 02/03/2023.
//

#ifndef OPENGLGP_RENDERER_H
#define OPENGLGP_RENDERER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "include/Components/Component.h"

class Model;

class Renderer : public Component {
private:
    std::shared_ptr<Model> model;
public:
    std::string path;
    glm::vec3 objectColor;
    float shininess;

public:
    Renderer(const std::shared_ptr <GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id);
    ~Renderer() override;


    void Update() override;
    void LoadModel(std::string newPath);

private:
    void Draw();
};


#endif //OPENGLGP_RENDERER_H
