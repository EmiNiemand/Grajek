//
// Created by szymo on 01/03/2023.
//

#ifndef OPENGLGP_GAMEOBJECT_H
#define OPENGLGP_GAMEOBJECT_H

#include "include/ProjectSettings.h"
#include "include/Components/Transform.h"
#include <memory>
#include <string>
#include <map>

class GloomEngine;
class Component;

class GameObject : public std::enable_shared_from_this<GameObject> {
private:
    std::shared_ptr<GloomEngine> gloomEngine;
    std::map<ComponentNames,std::shared_ptr<Component>> components;
    std::map<std::string, std::shared_ptr<GameObject>> children;

    // Name should be original for every game object, do not repeat
    std::string name;
public:
    std::shared_ptr<GameObject> parent = nullptr;

    // Create file with tag names as enums
    Tags tag;

    Transform transform;

    GameObject(const std::shared_ptr <GloomEngine> &gloomEngine, const std::string &name,
               const std::shared_ptr <GameObject> &parent = nullptr, Tags tag = Tags::DEFAULT);

    virtual ~GameObject();

    const std::shared_ptr<Component> &FindComponent(ComponentNames componentName);
    void AddComponent(std::shared_ptr<Component> &component);
    void RemoveComponent(ComponentNames componentName);
    void RemoveAllComponents();

    void SetParent(const std::shared_ptr<GameObject> &newParent);
    void AddChild(const std::shared_ptr<GameObject> &child);
    void RemoveChild(std::string childName);
    void RemoveAllChildren();


    void UpdateSelfAndChildren();
    void ForceUpdateSelfAndChildren();

    const std::string &GetName() const;
};


#endif //OPENGLGP_GAMEOBJECT_H
