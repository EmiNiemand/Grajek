//
// Created by masterktos on 06.04.23.
//

#include "Components/Scripts/SessionUI/SessionUI.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Animations/UIAnimator.h"

SessionUI::SessionUI(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void SessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples,
                      bool sessionMetronomeSound, bool sessionMetronomeVisuals, bool sessionBackingTrack) {
    // I negate actual values to use Toggle methods to easily setup elements
    metronomeSoundEnabled = !(sessionMetronomeSound & sessionMetronomeVisuals);
    metronomeVisualEnabled = !sessionMetronomeVisuals;
    backingTrackEnabled = !sessionBackingTrack;

    GameObject::Instantiate("Background", parent)
            ->AddComponent<Image>()->LoadTexture(0, 0, "UI/Sesja/vignetteBackground.png", 0.8);

    MetronomeSetup("UI/Sesja/Ramka.png", bpm);
    AccuracyFeedbackSetup();

    ToggleMetronomeVisuals();
    ToggleBackingTrack();

    // Set up sound samples
    // --------------------
    for (const auto& sample: samples)
    {
        sampleSources.push_back(GameObject::Instantiate("SampleSource", parent)->AddComponent<AudioSource>());
        sampleSources.back()->LoadAudioData(sample->clipPath, AudioType::Direct);

        sampleImages.push_back(GameObject::Instantiate("SampleImage", parent)
                                    ->AddComponent<Image>());
        sampleImages.back()->pivot = {0.5, 0.5};
    }
}

void SessionUI::SetCheatSheet(const std::string& cheatSheetPath) {
    cheatSheet = GameObject::Instantiate("CheatSheet", parent)->AddComponent<Image>();
    cheatSheet->LoadTexture(-985, 0, cheatSheetPath, -0.8);
}

void SessionUI::SetInstrumentControl(const std::string &instrumentControlPath) {
    instrumentControl = GameObject::Instantiate("InstrumentControl", parent)->AddComponent<Image>();
    instrumentControl->LoadTexture(1785, 0, instrumentControlPath, -0.8);
}

void SessionUI::PlaySound(int index) {
    sampleSources[index]->ForcePlaySound();
    //spdlog::info("[SUI] Played sound at index "+std::to_string(index)+"!");
}

bool SessionUI::ToggleCheatSheet() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("CheatSheetAnimator")) return false;
    if (instrumentControlActive) return false;
    cheatSheetActive = !cheatSheetActive;
    if (cheatSheetActive) {
        GameObject::Instantiate("CheatSheetAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(cheatSheet, {
                        {AnimatedProperty::Position, glm::vec3(-75, 0, 0), 0.5f}
                });
    } else {
        GameObject::Instantiate("CheatSheetAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(cheatSheet, {
                        {AnimatedProperty::Position, glm::vec3(-985, 0, 0), 0.5f}
                });
    }
    return true;
}

void SessionUI::ToggleInstrumentControl() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("InstrumentControlAnimator")) return;
    if (cheatSheetActive) return;
    instrumentControlActive = !instrumentControlActive;
    if (instrumentControlActive) {
        GameObject::Instantiate("InstrumentControlAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(instrumentControl, {
                {AnimatedProperty::Position, glm::vec3(885, 0, 0), 0.5f}
        });
    } else {
        GameObject::Instantiate("InstrumentControlAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(instrumentControl, {
                {AnimatedProperty::Position, glm::vec3(1785, 0, 0), 0.5f}
        });
    }
}

void SessionUI::UpdateAccuracy(float fraction) {
    int index = 0;
    for(auto threshold : accuracyThresholds)
    {
        if(fraction > threshold) index++;
        else break;
    }

    accuracyRatingAnimator[index]->Reset();
    //accuracyFeedback->text = accuracyTexts[index];
    //spdlog::info("[SUI] Accuracy rating:" + accuracyTexts[index]);
}

void SessionUI::Update() {
    if (metronomeSoundEnabled && metronomeImage->GetAlpha() == 1.0f) {
        tickSound->PlaySound();
    }
    Component::Update();
}

# pragma region Helper methods
void SessionUI::MetronomeSetup(const std::string& metronomePath, int bpm) {
    metronomeImage = GameObject::Instantiate("Metronome", parent)->AddComponent<Image>();
    metronomeImage->LoadTexture(0, 0, metronomePath, -0.5);
    metronomeAnimator = GameObject::Instantiate("MetronomeAnimator", parent)->AddComponent<UIAnimator>();
    metronomeAnimator->Setup(metronomeImage, {
            {AnimatedProperty::Alpha, glm::vec3(0.2f), 30.0f / (float)bpm},
            {AnimatedProperty::Alpha, glm::vec3(1.0f), 30.0f / (float)bpm}
    }, AnimationBehaviour::Looping);

    tickSound = metronomeImage->GetParent()->AddComponent<AudioSource>();
    tickSound->LoadAudioData("res/sounds/direct/tick.wav", AudioType::Direct);
    tickSound->IsLooping(false);

    metronomeSoundIndicator.insert({true, GameObject::Instantiate("MetronomeSoundEnabled", parent)->AddComponent<Image>()});
    metronomeSoundIndicator.insert({false, GameObject::Instantiate("MetronomeSoundDisabled", parent)->AddComponent<Image>()});
    metronomeSoundIndicator[true]->LoadTexture(93, 841, "UI/Sesja/metronomeSoundEnabled.png", -0.5);
    metronomeSoundIndicator[false]->LoadTexture(93, 841, "UI/Sesja/metronomeSoundDisabled.png", -0.5);
    metronomeSoundIndicator[true]->SetAlpha(metronomeSoundEnabled ? 1:0);
    metronomeSoundIndicator[false]->SetAlpha(metronomeSoundEnabled ? 0:1);

    metronomeVisualsIndicator.insert({true, GameObject::Instantiate("MetronomeVisualsEnabled", parent)->AddComponent<Image>()});
    metronomeVisualsIndicator.insert({false, GameObject::Instantiate("MetronomeVisualsDisabled", parent)->AddComponent<Image>()});
    metronomeVisualsIndicator[true]->LoadTexture(43, 840, "UI/Sesja/metronomeVisualsEnabled.png", -0.5);
    metronomeVisualsIndicator[false]->LoadTexture(43, 840, "UI/Sesja/metronomeVisualsDisabled.png", -0.5);
    metronomeVisualsIndicator[true]->SetAlpha(metronomeVisualEnabled ? 1:0);
    metronomeVisualsIndicator[false]->SetAlpha(metronomeVisualEnabled ? 0:1);
}

void SessionUI::BackingTrackSetup(const std::string& trackName) {
    backingTrack = GameObject::Instantiate("BackingTrack", parent)->AddComponent<AudioSource>();
    backingTrack->LoadAudioData("res/sounds/direct/"+trackName+".wav", AudioType::Direct);
    backingTrack->IsLooping(true);
    backingTrack->PlaySoundAfterStart(true);

    backingTrackIndicator.insert({true, GameObject::Instantiate("MetronomeVisualsEnabled", parent)->AddComponent<Image>()});
    backingTrackIndicator.insert({false, GameObject::Instantiate("MetronomeVisualsDisabled", parent)->AddComponent<Image>()});
    backingTrackIndicator[true]->LoadTexture(45, 790, "UI/Sesja/backingTrackEnabled.png", -0.5);
    backingTrackIndicator[false]->LoadTexture(45, 790, "UI/Sesja/backingTrackDisabled.png", -0.5);
    backingTrackIndicator[true]->SetAlpha(backingTrackEnabled ? 1:0);
    backingTrackIndicator[false]->SetAlpha(backingTrackEnabled ? 0:1);
}

void SessionUI::AccuracyFeedbackSetup() {
    accuracyRatingAnimator = {
            GameObject::Instantiate("AccuracyPoorAnimator", parent)->AddComponent<UIAnimator>(),
            GameObject::Instantiate("AccuracyNiceAnimator", parent)->AddComponent<UIAnimator>(),
            GameObject::Instantiate("AccuracyGreatAnimator", parent)->AddComponent<UIAnimator>(),
            GameObject::Instantiate("AccuracyPerfectAnimator", parent)->AddComponent<UIAnimator>()
    };
    std::string accuracyImagePaths[] = {
            "UI/Sesja/accuracyPoor.png", "UI/Sesja/accuracyNice.png",
            "UI/Sesja/accuracyAwesome.png", "UI/Sesja/AccuracyPerfect.png"
    };
    for (int i = 0; i < 4; ++i) {
        auto ratingImage = GameObject::Instantiate("AccuracyImage", parent)->AddComponent<Image>();
        ratingImage->LoadTexture(0, 0,accuracyImagePaths[i], 0.7f);
        ratingImage->SetPosition(960 - ratingImage->GetWidth()/2, 750 - ratingImage->GetHeight()/2);
        ratingImage->SetAlpha(0);
        accuracyRatingAnimator[i]->Setup(ratingImage, {
                {AnimatedProperty::Alpha, glm::vec3(1.0f), 0},
                {AnimatedProperty::Alpha, glm::vec3(0.0f), 1}
        }, AnimationBehaviour::Resetable);
    }
}

void SessionUI::StopSound(int index) {}

bool SessionUI::ToggleMetronomeSound() {
    metronomeSoundEnabled = !metronomeSoundEnabled && metronomeVisualEnabled;

    metronomeSoundIndicator[true]->SetAlpha(metronomeSoundEnabled ? 1:0);
    metronomeSoundIndicator[false]->SetAlpha(metronomeSoundEnabled ? 0:1);

    return metronomeSoundEnabled;
}

bool SessionUI::ToggleMetronomeVisuals() {
    metronomeVisualEnabled = !metronomeVisualEnabled;
    metronomeAnimator->paused = !metronomeVisualEnabled;

    if(!metronomeVisualEnabled) metronomeImage->SetAlpha(0);

    metronomeVisualsIndicator[true]->SetAlpha(metronomeVisualEnabled ? 1:0);
    metronomeVisualsIndicator[false]->SetAlpha(metronomeVisualEnabled ? 0:1);

    ToggleMetronomeSound();

    return metronomeVisualEnabled;
}

bool SessionUI::ToggleBackingTrack() {
    if(!backingTrack) return false;

    backingTrackEnabled = !backingTrackEnabled;
    backingTrack->SetGain(backingTrackEnabled? 1:0);

    backingTrackIndicator[true]->SetAlpha(backingTrackEnabled ? 1:0);
    backingTrackIndicator[false]->SetAlpha(backingTrackEnabled ? 0:1);

    return backingTrackEnabled;
}

void SessionUI::ChangeActiveButton(glm::vec2 moveVector) {
    if (!cheatSheetActive) return;

    if (moveVector.y == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->up;
        activeButton->isActive = true;
    }
    if (moveVector.y == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->down;
        activeButton->isActive = true;
    }
}

void SessionUI::OnClick() {
    if (!cheatSheetActive) return;

    for (int i = 0; i < soundButtons.size(); i++) {
        patternsSounds[i]->StopSound();
    }
    for (int i = 0; i < soundButtons.size(); i++) {
        if (soundButtons[i]->isActive) {
            patternsSounds[i]->PlaySound();
            return;
        }
    }
}

void SessionUI::OnDestroy() {
    accuracyRating.clear();
    accuracyRatingAnimator.clear();
    metronomeImage.reset();
    metronomeAnimator.reset();
    tickSound.reset();
    metronomeSoundIndicator[true].reset();
    metronomeSoundIndicator[false].reset();
    metronomeVisualsIndicator[true].reset();
    metronomeVisualsIndicator[false].reset();
    backingTrackIndicator[true].reset();
    backingTrackIndicator[false].reset();
    cheatSheet.reset();
    instrumentControl.reset();
    sampleSources.clear();
    sampleImages.clear();
    for(int i=0; i<sampleAnimators.size(); i++)
        sampleAnimators[i].clear();
    for(int i=0; i<patternsSounds.size(); i++)
        patternsSounds[i].reset();
    sampleAnimators.clear();
    Component::OnDestroy();
}
#pragma endregion