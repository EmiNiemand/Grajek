//
// Created by szymo on 17/06/2023.
//

#include "GameObjectsAndPrefabs/Prefabs/ConeIndicator.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Animations/GameObjectAnimator.h"

ConeIndicator::ConeIndicator(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag)
        : Prefab(name, id, parent, tag) {
    prefabName = "ConeIndicator";
}

ConeIndicator::~ConeIndicator() = default;

std::shared_ptr<GameObject> ConeIndicator::Create() {
    auto indicator = shared_from_this();
    auto renderer = indicator->AddComponent<Renderer>();
    renderer->LoadModel("Indicator/Indicator.obj");
    renderer->drawShadows = false;
    auto positionAnimator = GameObject::Instantiate("PositionAnimator", indicator);
    positionAnimator->AddComponent<GameObjectAnimator>()->Setup(indicator->transform, {
            {AnimatedProperty::Position, glm::vec3(0, -0.5f, 0), 1}, {AnimatedProperty::Position, glm::vec3(0, 0.5f, 0), 1}
    }, true);
    auto rotationAnimator = GameObject::Instantiate("PositionAnimator", indicator);
    rotationAnimator->AddComponent<GameObjectAnimator>()->Setup(indicator->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0, 360, 0), 5}}, true);

    return indicator;
}