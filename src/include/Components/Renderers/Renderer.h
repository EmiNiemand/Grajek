#ifndef OPENGLGP_RENDERER_H
#define OPENGLGP_RENDERER_H

#include "Drawable.h"

class StaticModel;

class Renderer : public Drawable {
private:
    std::shared_ptr<StaticModel> model;

public:
    inline static std::unordered_map<int, std::shared_ptr<StaticModel>> models;

public:
    Renderer(const std::shared_ptr<GameObject> &parent, int id);
    ~Renderer() override;


    void Update() override;
    void Draw() override;
    void Draw(std::shared_ptr<Shader> shader) override;

    void LoadModel(std::string path);
};


#endif //OPENGLGP_RENDERER_H
