#ifndef OPENGLGP_RENDERER_H
#define OPENGLGP_RENDERER_H

#include "Drawable.h"

class Model;

class Renderer : public Drawable {
private:
    inline static std::unordered_map<uint32_t, std::shared_ptr<Model>> models;
    std::shared_ptr<Model> model;

public:
    Renderer(const std::shared_ptr<GameObject> &parent, int id);
    ~Renderer() override;


    void Update() override;
    void Draw() override;
    void Draw(std::shared_ptr<Shader> shader) override;

    void LoadModel(std::string path);

    int TempBindTexture();
};


#endif //OPENGLGP_RENDERER_H
