#ifndef OPENGLGP_ENGINERENDERER_H
#define OPENGLGP_ENGINERENDERER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <map>

class GloomEngine;
class Shader;
class Component;
class PointLight;
class DirectionalLight;
class SpotLight;

class EngineRenderer {
public:
    float fov = 45.0f;
    glm::mat4 projection;


    // pair of id and ptr to light
    std::map<int, std::shared_ptr<PointLight>> pointLights;
    std::map<int, std::shared_ptr<DirectionalLight>> directionalLights;
    std::map<int, std::shared_ptr<SpotLight>> spotLights;

    std::shared_ptr<GloomEngine> gloomEngine;
    std::shared_ptr<Shader> shader;

public:
    explicit EngineRenderer(const std::shared_ptr<GloomEngine> &gloomEngine);
    virtual ~EngineRenderer();

    void Free();

    void UpdateRenderer();
    void UpdateProjection();
    void UpdateCamera();

    void UpdateLight(int componentId);
    void RemoveLight(int componentId);
    void SetFov(float fov);
private:
    void UpdatePointLight(int id);
    void UpdateDirectionalLight(int id);
    void UpdateSpotLight(int id);
    void RemovePointLight(int id);
    void RemoveDirectionalLight(int id);
    void RemoveSpotLight(int id);
};


#endif //OPENGLGP_ENGINERENDERER_H
