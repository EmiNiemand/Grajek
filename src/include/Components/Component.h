#ifndef OPENGLGP_COMPONENT_H
#define OPENGLGP_COMPONENT_H

#include "ProjectSettings.h"
#include <memory>
#include <string>

class GloomEngine;
class GameObject;

class Component : public std::enable_shared_from_this<Component> {
protected:
    int id;
    std::shared_ptr<GameObject> parent;

public:
    bool callOnAwake = true;
    bool callOnStart = true;
    bool enabled = true;

    Component(const std::shared_ptr<GameObject> &parent, int id);
    virtual ~Component() = 0;

    static void Destroy(const std::shared_ptr<Component>& component);

    /// Called one when Component is created by GameObject class
    inline virtual void OnCreate(){};
    /// Called one when Component is removed by GameObject class
    inline virtual void OnDestroy(){};

    /// Called once on creation even if disabled
    inline virtual void Awake(){callOnAwake = false;};
    /// Called once on creation if enabled
    inline virtual void Start(){callOnStart = false;};
    /// Called with 60Hz rate
    inline virtual void Update(){};
    /// Called with 120Hz rate
    inline virtual void FixedUpdate(){};
    /// Called with 2Hz rate
    inline virtual void AIUpdate(){};
    /// Called on game object transform change
    inline virtual void OnUpdate(){};

    inline virtual void OnTriggerEnter(const std::shared_ptr<GameObject>& gameObject){};
    inline virtual void OnTriggerStay(const std::shared_ptr<GameObject>& gameObject){};
    inline virtual void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject){};

    // Getters
    [[nodiscard]] int GetId() const;
    [[nodiscard]] const std::shared_ptr<GameObject> &GetParent() const;
};


#endif //OPENGLGP_COMPONENT_H
