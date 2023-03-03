//
// Created by szymo on 03/03/2023.
//

#ifndef OPENGLGP_ENGINERENDERER_H
#define OPENGLGP_ENGINERENDERER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <map>

class GloomEngine;
class Shader;
class PointLight;
class DirectionalLight;
class SpotLight;

class EngineRenderer {
private:
    float fov = 45.0f;
    glm::mat4 projection;

public:
    int pointLightNumber = 0;
    int spotLightNumber = 0;
    int directionalLightNumber = 0;

    // pair of id and ptr to light
    std::map<int, std::shared_ptr<PointLight>> pointLights;
    std::map<int, std::shared_ptr<DirectionalLight>> directionalLights;
    std::map<int, std::shared_ptr<SpotLight>> spotLights;

    std::shared_ptr<GloomEngine> gloomEngine;
    std::shared_ptr<Shader> shader;

public:
    explicit EngineRenderer(const std::shared_ptr<GloomEngine> &gloomEngine);
    virtual ~EngineRenderer();

    void Destroy();

    void UpdateRenderer();
    void UpdateProjection();
    void UpdateCamera();

    void UpdatePointLight(int id);
    void UpdateDirectionalLight(int id);
    void UpdateSpotLight(int id);

    void RemovePointLight(int id);
    void RemoveDirectionalLight(int id);
    void RemoveSpotLight(int id);

    void SetFov(float fov);

};


#endif //OPENGLGP_ENGINERENDERER_H
