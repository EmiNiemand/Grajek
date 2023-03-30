#ifndef OPENGLGP_COMPONENTFACTORY_H
#define OPENGLGP_COMPONENTFACTORY_H

#include "ProjectSettings.h"

#include <memory>
#include <string>

class GloomEngine;
class GameObject;

// Component classes - add component classes after creating new component
class Component;
class Renderer;
class CubeMap;
class Animator;
class Camera;
class PointLight;
class DirectionalLight;
class SpotLight;
class BoxCollider;
class Rigidbody;
class Image;
class Text;
class PlayerManager;
class PlayerMovement;

class ComponentFactory {
private:
    int id = 0;
    static ComponentFactory* componentFactory;

public:
    ComponentFactory(ComponentFactory &other) = delete;
    void operator=(const ComponentFactory&) = delete;

    static ComponentFactory* GetInstance();

    std::shared_ptr<Component> CreateComponent(std::string type, const std::shared_ptr<GameObject> &parent);

    // Creators - add creator for every component class
    std::shared_ptr<Renderer> CreateRenderer(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<CubeMap> CreateCubeMap(const std::shared_ptr<GameObject> &parent);
	std::shared_ptr<Animator> CreateAnimator(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<Camera> CreateCamera(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<PointLight> CreatePointLight(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<DirectionalLight> CreateDirectionalLight(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<SpotLight> CreateSpotLight(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<BoxCollider> CreateBoxCollider(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<Rigidbody> CreateRigidbody(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<Image> CreateImage(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<Text> CreateText(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<PlayerManager> CreatePlayerManager(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<PlayerMovement> CreatePlayerMovement(const std::shared_ptr<GameObject> &parent);

private:
    explicit ComponentFactory();
};


#endif //OPENGLGP_COMPONENTFACTORY_H
