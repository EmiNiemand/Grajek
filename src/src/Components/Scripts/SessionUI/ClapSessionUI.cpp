#include "Components/Scripts/SessionUI/ClapSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

ClapSessionUI::ClapSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void ClapSessionUI::PlaySound(int index) {
    auto circle1 = GloomEngine::GetInstance()->FindGameObjectWithName("Circle1")->GetComponent<Image>();
    auto circle2 = GloomEngine::GetInstance()->FindGameObjectWithName("Circle2")->GetComponent<Image>();
    auto circleAnimator = GameObject::Instantiate("CircleAnimator", parent->parent);
    auto circleAnimator2 = GameObject::Instantiate("CircleAnimator", parent->parent);
    if (index == 0) {
        circleAnimator->AddComponent<UIAnimator>()->Setup(circle1, {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
        }, false);
        circleAnimator2->AddComponent<UIAnimator>()->Setup(circle1, {
                {AnimatedProperty::Color, glm::vec3(0.0f, 0.0f, 1.0f), 0.125f},
                {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
        }, false);
    } else {
        circleAnimator->AddComponent<UIAnimator>()->Setup(circle2, {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
        }, false);
        circleAnimator2->AddComponent<UIAnimator>()->Setup(circle2, {
                {AnimatedProperty::Color, glm::vec3(1.0f, 0.0f, 0.0f), 0.125f},
                {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
        }, false);
    }
    SessionUI::PlaySound(index);
}