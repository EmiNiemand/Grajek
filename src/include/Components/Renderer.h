//
// Created by szymo on 02/03/2023.
//

#ifndef OPENGLGP_RENDERER_H
#define OPENGLGP_RENDERER_H

#include "Component.h"
#include "include/LowLevelClasses/Model.h"

class Renderer : public Component {
private:
    Shader shader = Shader("res/shaders/basic.vert", "res/shaders/basic.frag");
    std::shared_ptr<Model> model;
public:
    std::string path;

public:
    Renderer(const std::shared_ptr <GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent);
    ~Renderer() override;

    void Update() override;
    void LoadModel(std::string newPath);

private:
    void Draw();
};


#endif //OPENGLGP_RENDERER_H
