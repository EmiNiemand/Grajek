//
// Created by Adrian on 15.05.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Indicator.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"
#include "Components/Renderers/Camera.h"

Indicator::Indicator(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

Indicator::~Indicator() = default;

void Indicator::OnCreate() {
    camera = Camera::activeCamera;
    Component::OnCreate();
}

void Indicator::SetTexturePath(const char* path) {
    texturePath = path;
}

void Indicator::Indicate() {
    glm::vec3 cameraPos = camera->transform->GetLocalPosition();
    glm::vec3 chPos = parent->transform->GetLocalPosition();
    x = (chPos.x - cameraPos.x + 19.0f) * 50.5f;
    y = -(chPos.z - cameraPos.z + 34.0f) * 27.0f + 1080.0f;

    auto animator = GameObject::Instantiate("Animator", parent);
    auto indicator = GameObject::Instantiate("Indicator", animator)->
            AddComponent<Image>();
    indicator->LoadTexture((int)x, (int)y, texturePath);
    animator->AddComponent<UIAnimator>()->Setup(indicator, {
            {AnimatedProperty::Position, glm::vec3(x, y + 50.0f, 0.0f)}
    }, false);
}
