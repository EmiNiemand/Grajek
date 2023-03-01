//
// Created by szymo on 01/03/2023.
//

#ifndef OPENGLGP_GAMEOBJECT_H
#define OPENGLGP_GAMEOBJECT_H

#include "include/ProjectSettings.h"
#include "include/Components/Transform.h"
#include "include/Components/Component.h"
#include <memory>
#include <map>
#include <vector>
#include <string>


class GameObject : public std::enable_shared_from_this<GameObject> {
private:
    std::map<ComponentNames,std::shared_ptr<Component>> components;
    std::map<std::string, std::shared_ptr<GameObject>> children;

    // Name should be original for every game object, do not repeat
    std::string name;
public:
    std::shared_ptr<GameObject> parent = nullptr;

    // Create file with tag names as enums
    Tags tag;

    Transform transform;

    GameObject(const std::string &name, const Tags &tag, const Transform &transform = Transform());
    virtual ~GameObject();

    void AddComponent(std::shared_ptr<Component> component);
    void RemoveComponent(ComponentNames componentName);

    void AddChild(std::shared_ptr<GameObject> child);
    void RemoveChild(std::string childName);

    void UpdateSelfAndChildren();
    void ForceUpdateSelfAndChildren();

    const std::string &GetName() const;
};


#endif //OPENGLGP_GAMEOBJECT_H
