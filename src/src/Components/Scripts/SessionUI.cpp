//
// Created by masterktos on 06.04.23.
//

#include "Components/Scripts/SessionUI.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Utilities.h"

SessionUI::SessionUI(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void SessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>>& samples, std::shared_ptr<Image> metronome) {
    metronomeSoundEnabled = true;
    metronomeVisualEnabled = true;

    metronomeImage = std::move(metronome);

    accuracyFeedback = GameObject::Instantiate("AccuracyFeedback", parent)->AddComponent<Text>();
    accuracyFeedback->LoadFont("Good", 500, 1280, 20, glm::vec3(1.0f), GameFont::Eggnog);

    //TODO: uncomment when sound gets implemented
//        sampleSources = new List<AudioSource>();
//        foreach (auto sample: samples)
//        {
//            sampleSources.Add(GameObject::Instantiate("Sample", parent)<AudioSource>());
//            sampleSources.back().clip = sample.clip;
//        }
}

void SessionUI::SetCheatSheet(std::shared_ptr<Image> newCheatSheet) { cheatSheet = std::move(newCheatSheet); }

void SessionUI::PlaySound(int index) {
    //TODO: play some kind of visual confirmation of playing sound
//        animator.SetTrigger("Sound"+index);
    //TODO: play the actual sound when it's implemented
//        sampleSources[index].Play();
    spdlog::info("[SUI] Played sound at index "+std::to_string(index)+"!");
}

void SessionUI::ToggleCheatSheet() { cheatSheet->enabled = cheatSheet->enabled; }

void SessionUI::UpdateAccuracy(float fraction) {
    int index = 0;
    for(auto threshold : accuracyThresholds)
    {
        if(fraction > threshold) index++;
        else break;
    }

    //TODO: Show accuracy feedback when changing text is implemented
//        accuracyFeedback.text = accuracyTexts[index];
//        accuracyFeedback.color = accuracyColors[index];
    spdlog::info("[SUI] Accuracy rating:" + accuracyTexts[index]);
}
