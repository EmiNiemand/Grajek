//
// Created by masterktos on 06.04.23.
//

#include "Components/Scripts/SessionUI.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Audio/AudioSource.h"

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
        sampleSources.back()->LoadAudioData(sample->clipPath.c_str(), AudioType::Music);
    }
}

void SessionUI::SetCheatSheet(std::shared_ptr<Image> newCheatSheet) { cheatSheet = std::move(newCheatSheet); }

void SessionUI::PlaySound(int index) {
    //TODO: play some kind of visual confirmation of playing sound
    //animator.SetTrigger("Sound"+index);
    sampleSources[index]->PlaySound();
    spdlog::info("[SUI] Played sound at index "+std::to_string(index)+"!");
}

void SessionUI::ToggleCheatSheet() { cheatSheet->enabled = !cheatSheet->enabled; }

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
