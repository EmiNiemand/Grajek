#ifndef OPENGLGP_COMPONENTFACTORY_H
#define OPENGLGP_COMPONENTFACTORY_H

#include "ProjectSettings.h"
#include "GloomEngine.h"

#include <memory>
#include <string>

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
class PlayerUI;
class PlayerEquipment;
class SessionUI;
class MusicSession;

class ComponentFactory {
private:
    int id = 0;
    inline static ComponentFactory* componentFactory;

public:
    ComponentFactory(ComponentFactory &other) = delete;
    void operator=(const ComponentFactory&) = delete;

    static ComponentFactory* GetInstance();

    template<class T>
    std::shared_ptr<T> CreateComponent(const std::shared_ptr<GameObject> &parent) {
        id++;
        std::shared_ptr<T> component = std::make_shared<T>(parent, id);
        std::shared_ptr<T> parentComponent = std::dynamic_pointer_cast<T>(component);
        return component;
    };

private:
    explicit ComponentFactory();
};


#endif //OPENGLGP_COMPONENTFACTORY_H
