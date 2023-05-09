#include "Components/Scripts/SessionUI/DrumsSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"
#include "EngineManagers/RandomnessManager.h"

DrumsSessionUI::DrumsSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void DrumsSessionUI::PlaySound(int index) {
    auto animator = GameObject::Instantiate("NutaAnimator", parent->parent);
    auto nuta = GameObject::Instantiate("Nuta", animator)->AddComponent<Image>();
    auto circle1 = GloomEngine::GetInstance()->FindGameObjectWithName("Circle1")->GetComponent<Image>();
    auto circle2 = GloomEngine::GetInstance()->FindGameObjectWithName("Circle2")->GetComponent<Image>();
    auto circle3 = GloomEngine::GetInstance()->FindGameObjectWithName("Circle3")->GetComponent<Image>();
    auto circleAnimator = GameObject::Instantiate("CircleAnimator", parent->parent);
    auto circleAnimator2 = GameObject::Instantiate("CircleAnimator", parent->parent);
    if (index == 0) {
        int random = RandomnessManager::GetInstance()->GetInt(500, 700);
        nuta->LoadTexture(random, 675, "UI/Sesja/Nuta1.png", -0.1f);
        GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Scale, glm::vec3(0.5f)}
        }, false);
        animator->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(random, 750.0f, 0.0f)}
        }, false);
        GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Rotation, glm::vec3(0.0f, 0.0f, RandomnessManager::GetInstance()->GetFloat(-45.0f, 45.0f))}
        }, false);
        circleAnimator->AddComponent<UIAnimator>()->Setup(circle1, {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
        }, false);
        circleAnimator2->AddComponent<UIAnimator>()->Setup(circle1, {
                {AnimatedProperty::Color, glm::vec3(0.0f, 0.0f, 1.0f), 0.125f},
                {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
        }, false);
    } else if (index == 1) {
        int random = RandomnessManager::GetInstance()->GetInt(150, 450);
        nuta->LoadTexture(625, random, "UI/Sesja/Nuta2.png", -0.1f);
        GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Scale, glm::vec3(0.5f)}
        }, false);
        animator->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(700.0f, random, 0.0f)}
        }, false);
        GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Rotation, glm::vec3(0.0f, 0.0f, RandomnessManager::GetInstance()->GetFloat(-45.0f, 45.0f))}
        }, false);
        circleAnimator->AddComponent<UIAnimator>()->Setup(circle2, {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
        }, false);
        circleAnimator2->AddComponent<UIAnimator>()->Setup(circle2, {
                {AnimatedProperty::Color, glm::vec3(1.0f, 0.0f, 0.0f), 0.125f},
                {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
        }, false);
    } else if (index == 2) {
        int random = RandomnessManager::GetInstance()->GetInt(250, 450);
        nuta->LoadTexture(random, 475, "UI/Sesja/Nuta3.png", -0.1f);
        GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Scale, glm::vec3(0.5f)}
        }, false);
        animator->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(random, 550.0f, 0.0f)}
        }, false);
        GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(nuta, {
                {AnimatedProperty::Rotation, glm::vec3(0.0f, 0.0f, RandomnessManager::GetInstance()->GetFloat(-45.0f, 45.0f))}
        }, false);
        circleAnimator->AddComponent<UIAnimator>()->Setup(circle3, {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
        }, false);
        circleAnimator2->AddComponent<UIAnimator>()->Setup(circle3, {
                {AnimatedProperty::Color, glm::vec3(0.0f, 1.0f, 0.0f), 0.125f},
                {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
        }, false);
    }

    SessionUI::PlaySound(index);
}