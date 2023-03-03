//
// Created by szymo on 02/03/2023.
//

#ifndef OPENGLGP_COMPONENTFACTORY_H
#define OPENGLGP_COMPONENTFACTORY_H

#include "include/ProjectSettings.h"
#include <memory>
#include <string>

class GloomEngine;
class GameObject;

// Component classes - add component classes after creating new component
class Renderer;
class Camera;
class PointLight;
class DirectionalLight;
class SpotLight;

class ComponentFactory {
private:
    std::shared_ptr<GloomEngine> gloomEngine;
public:
    explicit ComponentFactory(const std::shared_ptr<GloomEngine> &gloomEngine);

    // Creators - add creator for every component class
    std::shared_ptr<Renderer> CreateRenderer(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<Camera> CreateCamera(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<PointLight> CreatePointLight(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<DirectionalLight> CreateDirectionalLight(const std::shared_ptr<GameObject> &parent);
    std::shared_ptr<SpotLight> CreateSpotLight(const std::shared_ptr<GameObject> &parent);
};


#endif //OPENGLGP_COMPONENTFACTORY_H
