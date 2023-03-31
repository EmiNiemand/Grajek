#ifndef OPENGLGP_RENDERERMANAGER_H
#define OPENGLGP_RENDERERMANAGER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <map>

class GloomEngine;
class Shader;
class GameObject;
class Component;
class PointLight;
class DirectionalLight;
class SpotLight;

class RendererManager {
private:
    static RendererManager* rendererManager;

public:
    float fov = 45.0f;
    glm::mat4 projection{};

    // pair of id and ptr to light
    std::map<int, std::shared_ptr<PointLight>> pointLights;
    std::map<int, std::shared_ptr<DirectionalLight>> directionalLights;
    std::map<int, std::shared_ptr<SpotLight>> spotLights;

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> cubeMapShader;
	std::shared_ptr<Shader> animatedShader;

public:
    RendererManager(RendererManager &other) = delete;
    void operator=(const RendererManager&) = delete;
    virtual ~RendererManager();

    static RendererManager* GetInstance();

    void Free() const;

    void UpdateRenderer() const;
    void UpdateProjection() const;
    void UpdateCamera() const;

    void UpdateLight(int componentId);
    void RemoveLight(int componentId);
    void SetFov(float fov);
private:
    explicit RendererManager();

    void UpdatePointLight(int id, const std::shared_ptr<Shader>& lightShader);
    void UpdateDirectionalLight(int id, const std::shared_ptr<Shader>& lightShader);
    void UpdateSpotLight(int id, const std::shared_ptr<Shader>& lightShader);
    void RemovePointLight(int id, const std::shared_ptr<Shader>& lightShader);
    void RemoveDirectionalLight(int id, const std::shared_ptr<Shader>& lightShader);
    void RemoveSpotLight(int id, const std::shared_ptr<Shader>& lightShader);
};


#endif //OPENGLGP_RENDERERMANAGER_H
