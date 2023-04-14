#ifndef OPENGLGP_RENDERERMANAGER_H
#define OPENGLGP_RENDERERMANAGER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <vector>
#include <map>

class Shader;
class Drawable;
class PointLight;
class DirectionalLight;
class SpotLight;

class RendererManager {
private:
    inline static RendererManager* rendererManager;

public:
    float fov = 45.0f;
    glm::mat4 projection{};

    // pair of id and ptr to light
    std::map<int, std::shared_ptr<PointLight>> pointLights;
    std::map<int, std::shared_ptr<DirectionalLight>> directionalLights;
    std::map<int, std::shared_ptr<SpotLight>> spotLights;

    std::vector<std::shared_ptr<Drawable>> drawBuffer;

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> cubeMapShader;

public:
    RendererManager(RendererManager &other) = delete;
    void operator=(const RendererManager&) = delete;
    virtual ~RendererManager();

    static RendererManager* GetInstance();

    void Free() const;

    void DrawObjects();
    void AddToDrawBuffer(const std::shared_ptr<Drawable>& DrawableComponent);

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
