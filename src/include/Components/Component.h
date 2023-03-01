//
// Created by szymo on 01/03/2023.
//

#ifndef OPENGLGP_COMPONENT_H
#define OPENGLGP_COMPONENT_H

#include "include/ProjectSettings.h"
#include <memory>
#include <string>

class GameObject;

class Component {
private:
    ComponentNames name;
    std::shared_ptr<GameObject> parent = nullptr;
public:
    bool enabled = true;

    Component(const ComponentNames &name, const std::shared_ptr<GameObject> &parent);
    virtual ~Component() = 0;

    // Getters
    const ComponentNames &GetName() const;
    const std::shared_ptr<GameObject> &GetParent() const;

    //Setters
    void SetParent(const std::shared_ptr<GameObject> &parent);
};


#endif //OPENGLGP_COMPONENT_H
