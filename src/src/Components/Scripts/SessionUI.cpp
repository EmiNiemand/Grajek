//
// Created by masterktos on 06.04.23.
//

#include "Components/Scripts/SessionUI.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Animations/UIAnimator.h"
#include "EngineManagers/RandomnessManager.h"

SessionUI::SessionUI(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void SessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, std::shared_ptr<Image> metronome) {
    metronomeSoundEnabled = true;
    metronomeVisualEnabled = true;

    metronomeImage = std::move(metronome);

    accuracyFeedback = GameObject::Instantiate("AccuracyFeedback", parent)->AddComponent<Text>();
    accuracyFeedback->LoadFont("Good", 960, 540, 60, Color::White, GameFont::KanitLight);

    for (const auto& sample: samples)
    {
        sampleSources.push_back(GameObject::Instantiate("Sample", parent)->AddComponent<AudioSource>());
        sampleSources.back()->LoadAudioData(sample->clipPath.c_str(), AudioType::Direct);
    }

    SetCheatSheet(GameObject::Instantiate("CheatSheet", parent->parent)->AddComponent<Image>());
    cheatSheet->LoadTexture(451, -1100, "UI/Sesja/drumPatterns.png", -0.2);
}

void SessionUI::SetCheatSheet(std::shared_ptr<Image> newCheatSheet) { cheatSheet = std::move(newCheatSheet); }

void SessionUI::PlaySound(int index) {
    //TODO: play some kind of visual confirmation of playing sound
    auto animator = GameObject::Instantiate("NutaAnimator", parent->parent);
    auto nuta = GameObject::Instantiate("Nuta", animator)->AddComponent<Image>();
    auto circle1 = GloomEngine::GetInstance()->FindGameObjectWithName("Circle1")->GetComponent<Image>();
    auto circle2 = GloomEngine::GetInstance()->FindGameObjectWithName("Circle2")->GetComponent<Image>();
    std::shared_ptr<Image> circle3;
    if (GloomEngine::GetInstance()->FindGameObjectWithName("Circle3")) {
        circle3 = GloomEngine::GetInstance()->FindGameObjectWithName("Circle3")->GetComponent<Image>();
    }
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
//    animator.SetTrigger("Sound"+index);
    sampleSources[index]->ForcePlaySound();
    spdlog::info("[SUI] Played sound at index "+std::to_string(index)+"!");
}

void SessionUI::ToggleCheatSheet() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("CheatSheetAnimator")) return;
    cheatSheetActive = !cheatSheetActive;
    if (cheatSheetActive) {
        GameObject::Instantiate("CheatSheetAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(
                cheatSheet, {
                        {AnimatedProperty::Position, glm::vec3(451.0f, -50.0f, 0.0f), 0.5f}
                }, false);
    } else {
        GameObject::Instantiate("CheatSheetAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(
                cheatSheet, {
                        {AnimatedProperty::Position, glm::vec3(451.0f, -1100.0f, 0.0f), 0.5f}
                }, false);
    }
}

void SessionUI::UpdateAccuracy(float fraction) {
    int index = 0;
    for(auto threshold : accuracyThresholds)
    {
        if(fraction > threshold) index++;
        else break;
    }

    //TODO: Show accuracy feedback when changing text is implemented
    accuracyFeedback->text = accuracyTexts[index];
    accuracyFeedback->color = accuracyColors[index];
    spdlog::info("[SUI] Accuracy rating:" + accuracyTexts[index]);
}
