//
// Created by masterktos on 06.04.23.
//

#include "Components/Scripts/SessionUI/SessionUI.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Animations/UIAnimator.h"
#include "EngineManagers/OptionsManager.h"

SessionUI::SessionUI(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void SessionUI::Setup(std::shared_ptr<Instrument> instrument) {
    // I negate actual values to use Toggle methods to easily setup elements
    metronomeSoundEnabled = !OptionsManager::GetInstance()->sessionMetronomeSound;
    metronomeVisualEnabled = !OptionsManager::GetInstance()->sessionMetronomeVisuals;
    backingTrackEnabled = !OptionsManager::GetInstance()->sessionBackingTrack;

    // TODO: ugly workaround, replace when possible by passing instrument to method
    std::string instrumentName = instrument->NameToString();
    std::string instrumentNameLCase = instrumentName;
    instrumentNameLCase[0] -= 'Z'-'z';

    // Load backing track
    // ------------------
    BackingTrackSetup(instrumentNameLCase+"/backingTrack");

    // Load background
    // ---------------
    GameObject::Instantiate("Background", parent)
            ->AddComponent<Image>()->LoadTexture(0, 0, "UI/Sesja/vignetteBackground.png", 0.8);

    MetronomeSetup( (int)instrument->genre);
    AccuracyFeedbackSetup();

    ToggleMetronomeVisuals();
    ToggleBackingTrack();

    // Set up cheat sheet
    // ------------------
    SetCheatSheet("UI/Sesja/"+instrumentName+"Patterns.png");

    // Set up instrument control
    // ------------------
    SetInstrumentControl("UI/Sesja/"+instrumentName+"Control.png");

    // Load theme
    // ----------
    SetTheme("UI/Sesja/widok"+instrumentName+".png");

    // Set up sound samples
    // --------------------
    for (const auto& sample: instrument->samples)
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
    cheatSheet->LoadTexture(1785, 0, cheatSheetPath, -0.8);
}

void SessionUI::SetInstrumentControl(const std::string &instrumentControlPath) {
    instrumentControl = GameObject::Instantiate("InstrumentControl", parent)->AddComponent<Image>();
    instrumentControl->LoadTexture(1785, 0, instrumentControlPath, -0.8);

    notesRevealSound = GameObject::Instantiate("NotesRevealSound", parent)->AddComponent<AudioSource>();
    notesRevealSound->LoadAudioData("res/sounds/direct/cheatsheet_up.wav", AudioType::Direct);
    notesHideSound = GameObject::Instantiate("NotesRevealSound", parent)->AddComponent<AudioSource>();
    notesHideSound->LoadAudioData("res/sounds/direct/cheatsheet_down.wav", AudioType::Direct);
}

void SessionUI::SetTheme(const std::string &themePath) {
    auto theme = GameObject::Instantiate("Theme", parent)->AddComponent<Image>();
    theme->LoadTexture(0, 0, themePath);
    theme->SetScale(0.8f);
}

void SessionUI::PlaySound(int index) {
    sampleSources[index]->ForcePlaySound();
    //spdlog::info("[SUI] Played sound at index "+std::to_string(index)+"!");
}

bool SessionUI::ToggleCheatSheet() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("CheatSheetAnimator")) return false;
    if (GloomEngine::GetInstance()->FindGameObjectWithName("InstrumentControlAnimator")) return false;
    if (instrumentControlActive) {
        instrumentControlActive = false;
        GameObject::Instantiate("InstrumentControlAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(instrumentControl, {
                {AnimatedProperty::Position, glm::vec3(1785, 0, 0), 0.5f}
        });
    }
    cheatSheetActive = !cheatSheetActive;
    if (cheatSheetActive) {
        notesRevealSound->ForcePlaySound();
        cheatSheet->SetZ(-0.81);
        instrumentControl->SetZ(-0.8);
        GameObject::Instantiate("CheatSheetAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(cheatSheet, {
                        {AnimatedProperty::Position, glm::vec3(885, 0, 0), 0.5f}
                });
        for (int i = 0; i < soundButtons.size(); i++) {
            soundButtons[i]->isActive = false;
            soundAnimators[i][0]->Reset();
        }
        soundButtons[0]->isActive = true;
        activeButton = soundButtons[0];
    } else {
        notesHideSound->ForcePlaySound();
        GameObject::Instantiate("CheatSheetAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(cheatSheet, {
                        {AnimatedProperty::Position, glm::vec3(1785, 0, 0), 0.5f}
                });
        for (int i = 0; i < soundButtons.size(); i++) {
            soundAnimators[i][1]->Reset();
        }
    }
    return true;
}

void SessionUI::ToggleInstrumentControl() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("CheatSheetAnimator")) return;
    if (GloomEngine::GetInstance()->FindGameObjectWithName("InstrumentControlAnimator")) return;
    if (cheatSheetActive) {
        cheatSheetActive = false;
        GameObject::Instantiate("CheatSheetAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(cheatSheet, {
                {AnimatedProperty::Position, glm::vec3(885, 0, 0), 0.5f}
        });
        for (int i = 0; i < soundAnimators.size(); i++) {
            soundAnimators[i][1]->Reset();
        }
    }
    instrumentControlActive = !instrumentControlActive;
    if (instrumentControlActive) {
        notesRevealSound->ForcePlaySound();
        instrumentControl->SetZ(-0.81);
        cheatSheet->SetZ(-0.8);
        GameObject::Instantiate("InstrumentControlAnimator", parent->parent)
                ->AddComponent<UIAnimator>()->Setup(instrumentControl, {
                {AnimatedProperty::Position, glm::vec3(885, 0, 0), 0.5f}
        });
    } else {
        notesHideSound->ForcePlaySound();
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
void SessionUI::MetronomeSetup(int bpm) {
    metronomeImage = GameObject::Instantiate("Metronome", parent)->AddComponent<Image>();
    metronomeImage->LoadTexture(0, 0, "UI/Sesja/MetronomeFrame.png", -0.5);
    metronomeMask = GameObject::Instantiate("Metronome", parent)->AddComponent<Image>();
    metronomeMask->LoadTexture(0, 0, "UI/Sesja/MetronomeFrameMask.png", -0.51);
    metronomeMask->SetAlpha(0);

    metronomeAnimator = GameObject::Instantiate("MetronomeAnimator", parent)->AddComponent<UIAnimator>();
    metronomeAnimator->Setup(metronomeImage, {
            // Duration of both checkpoints must add up to 60 to scale correctly with BPM
            {AnimatedProperty::Alpha, glm::vec3(0.2f), 58.0f / (float)bpm},
            {AnimatedProperty::Alpha, glm::vec3(1.0f), 2.0f / (float)bpm}
    }, AnimationBehaviour::Looping);

    tickSound = metronomeImage->GetParent()->AddComponent<AudioSource>();
    tickSound->LoadAudioData("res/sounds/direct/tick.wav", AudioType::Direct);
    tickSound->IsLooping(false);

    metronomeSoundIndicator = GameObject::Instantiate("MetronomeSoundDisabled", parent)->AddComponent<Image>();
    metronomeSoundIndicator->LoadTexture(1830, 250, "UI/Sesja/ToggleDisabled.png", -0.85);
    metronomeSoundIndicator->SetAlpha(metronomeSoundEnabled ? 0:1);

    metronomeVisualsIndicator = GameObject::Instantiate("MetronomeVisualsDisabled", parent)->AddComponent<Image>();
    metronomeVisualsIndicator->LoadTexture(1830, 200, "UI/Sesja/ToggleDisabled.png", -0.85);
    metronomeVisualsIndicator->SetAlpha(metronomeVisualEnabled ? 0:1);
}

void SessionUI::BackingTrackSetup(const std::string& trackName) {
    backingTrack = GameObject::Instantiate("BackingTrack", parent)->AddComponent<AudioSource>();
    backingTrack->LoadAudioData("res/sounds/direct/"+trackName+".wav", AudioType::Direct);
    backingTrack->IsLooping(true);
    backingTrack->PlaySoundAfterStart(true);

    backingTrackIndicator = GameObject::Instantiate("MetronomeVisualsDisabled", parent)->AddComponent<Image>();
    backingTrackIndicator->LoadTexture(1830, 150, "UI/Sesja/ToggleDisabled.png", -0.85);
    backingTrackIndicator->SetAlpha(backingTrackEnabled ? 0:1);
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
    metronomeSoundEnabled = !metronomeSoundEnabled;

    metronomeSoundIndicator->SetAlpha(metronomeSoundEnabled ? 0:1);
    OptionsManager::GetInstance()->sessionMetronomeSound = metronomeSoundEnabled;

    return metronomeSoundEnabled;
}

bool SessionUI::ToggleMetronomeVisuals() {
    metronomeVisualEnabled = !metronomeVisualEnabled;

    metronomeMask->SetAlpha(metronomeVisualEnabled ? 0:1);
    metronomeVisualsIndicator->SetAlpha(metronomeVisualEnabled ? 0:1);
    OptionsManager::GetInstance()->sessionMetronomeVisuals = metronomeVisualEnabled;

    return metronomeVisualEnabled;
}

bool SessionUI::ToggleBackingTrack() {
    if(!backingTrack) return false;

    backingTrackEnabled = !backingTrackEnabled;
    backingTrack->SetGain(backingTrackEnabled? 1:0);

    backingTrackIndicator->SetAlpha(backingTrackEnabled ? 0:1);

    OptionsManager::GetInstance()->sessionBackingTrack = backingTrackEnabled;

    return backingTrackEnabled;
}

void SessionUI::ChangeActiveButton(glm::vec2 moveVector) {
    if (!cheatSheetActive) return;
    if (!activeButton) return;

    if (moveVector.y == 1.0f) {
        if (!activeButton->up) return;
        activeButton->isActive = false;
        activeButton = activeButton->up;
        activeButton->isActive = true;
    }
    if (moveVector.y == -1.0f) {
        if (!activeButton->down) return;
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
    metronomeSoundIndicator.reset();
    metronomeVisualsIndicator.reset();
    backingTrackIndicator.reset();
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