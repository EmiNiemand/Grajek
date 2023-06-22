#ifndef OPENGLGP_COMPONENTFACTORY_H
#define OPENGLGP_COMPONENTFACTORY_H

#include "ProjectSettings.h"
#include "GloomEngine.h"

#include <memory>
#include <string>

class GameObject;

class ComponentFactory {
private:
    int id = 0;
    inline static ComponentFactory* componentFactory;

public:
    virtual ~ComponentFactory();

    ComponentFactory(ComponentFactory &other) = delete;
    void operator=(const ComponentFactory&) = delete;

    static ComponentFactory* GetInstance();

    template<class T>
    std::shared_ptr<T> CreateComponent(const std::shared_ptr<GameObject> &parent) {
        id++;
        std::shared_ptr<T> component = std::make_shared<T>(parent, id);
        GloomEngine::GetInstance()->AddComponent(component);
        component->OnCreate();
        return component;
    };

private:
    explicit ComponentFactory();
};


#endif //OPENGLGP_COMPONENTFACTORY_H
