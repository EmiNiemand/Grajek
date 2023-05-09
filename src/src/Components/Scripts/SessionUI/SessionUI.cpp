//
// Created by masterktos on 06.04.23.
//

#include "Components/Scripts/SessionUI/SessionUI.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Animations/UIAnimator.h"

SessionUI::SessionUI(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void SessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, std::shared_ptr<Image> metronome) {
    metronomeSoundEnabled = true;
    metronomeVisualEnabled = true;

    metronomeImage = std::move(metronome);
    GameObject::Instantiate("MetronomeAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(metronomeImage, {
            {AnimatedProperty::Alpha, glm::vec3(0.2f), 30.0f / (float)bpm},
            {AnimatedProperty::Alpha, glm::vec3(1.0f), 30.0f / (float)bpm}
    }, true);

    tickSound = parent->AddComponent<AudioSource>();
    tickSound->LoadAudioData("res/sounds/direct/tick.wav", AudioType::Direct);
    tickSound->IsLooping(false);

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

void SessionUI::Update() {
    if (metronomeImage->GetAlpha() == 1.0f) {
        tickSound->PlaySound();
    }
    Component::Update();
}
