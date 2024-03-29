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
    float zNear = 0.1f;
    float zFar = 100.0f;
    glm::mat4 projection{};

    // pair of id and ptr to light
    std::map<int, std::shared_ptr<PointLight>> pointLights;
    std::map<int, std::shared_ptr<DirectionalLight>> directionalLights;
    std::map<int, std::shared_ptr<SpotLight>> spotLights;

    unsigned int bufferIterator = 0;
    std::shared_ptr<Drawable> drawBuffer[1000];

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> cubeMapShader;

public:
    RendererManager(RendererManager &other) = delete;
    void operator=(const RendererManager&) = delete;
    virtual ~RendererManager();

    static RendererManager* GetInstance();

    void Free() const;

    void Draw();
    void DrawObjects();
    void DrawObjects(const std::shared_ptr<Shader>& drawShader);
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

    void ClearBuffer();
};


#endif //OPENGLGP_RENDERERMANAGER_H
