#ifndef OPENGLGP_COMPONENT_H
#define OPENGLGP_COMPONENT_H

#include "ProjectSettings.h"
#include <memory>
#include <string>

class GloomEngine;
class GameObject;

class Component {
protected:
    int id;
    std::shared_ptr<GameObject> parent;

public:
    bool callOnAwake = true;
    bool callOnStart = true;
    bool enabled = true;

    Component(const std::shared_ptr<GameObject> &parent, int id);
    virtual ~Component() = 0;

    /// Called once on creation even if disabled
    virtual void Awake(){callOnAwake = false;};
    /// Called once on creation if enabled
    virtual void Start(){callOnStart = false;};
    /// Called every frame
    virtual void Update(){};
    /// Called on game object transform change
    virtual void OnUpdate(){};

    // Getters
    [[nodiscard]] int GetId() const;
    [[nodiscard]] const std::shared_ptr<GameObject> &GetParent() const;

    //Setters
    void SetParent(const std::shared_ptr<GameObject> &parent);
};


#endif //OPENGLGP_COMPONENT_H
