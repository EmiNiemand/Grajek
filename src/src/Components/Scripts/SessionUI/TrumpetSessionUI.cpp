#include "Components/Scripts/SessionUI/TrumpetSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

TrumpetSessionUI::TrumpetSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void TrumpetSessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples,
                             bool sessionMetronomeSound, bool sessionMetronomeVisuals, bool sessionBackingTrack) {
    // Load backing track
    // ------------------
    BackingTrackSetup("trumpet/backingTrack");

    SessionUI::Setup(bpm, samples, sessionMetronomeSound, sessionMetronomeVisuals, sessionBackingTrack);

    // Set up instrument control
    // ------------------
    SetInstrumentControl("UI/Sesja/trumpetControl.png");

    // Add cheat sheet
    // ------------------
    SetCheatSheet("UI/Sesja/trumpetPatterns.png");

    // Load theme
    // ----------
    GameObject::Instantiate("Theme", parent)->AddComponent<Image>()
            ->LoadTexture(0, 0, "UI/Sesja/widokTrabka.png");

    valveInitPos[0] = {1100, 0};
    valveInitPos[1] = {1300, 0};
    valveInitPos[2] = {1500, 0};
    float positionAnimationDuration = 1.25f;

    // Set up samples
    // --------------
    // Low-pitched sound
    sampleImages[0]->LoadTexture(0, 0, "UI/Sesja/trumpetValve.png");
    sampleAnimators.push_back({
        GameObject::Instantiate("AlphaAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("PositionAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[0][0]->Setup(sampleImages[0], {
            {AnimatedProperty::Alpha, glm::vec3(0.6f), 0.2f},
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][1]->Setup(sampleImages[0], {
            {AnimatedProperty::Position, glm::vec3(valveInitPos[0].x, -sampleImages[0]->GetHeight()*0.9, 0), positionAnimationDuration}
    }, AnimationBehaviour::Resetable);

    // Medium-pitched sound
    sampleImages[1]->LoadTexture(0, 0, "UI/Sesja/trumpetValve.png");
    sampleAnimators.push_back({
        GameObject::Instantiate("AlphaAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("PositionAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[1][0]->Setup(sampleImages[1], {
            {AnimatedProperty::Alpha, glm::vec3(0.6f), 0.2f},
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][1]->Setup(sampleImages[1], {
            {AnimatedProperty::Position, glm::vec3(valveInitPos[1].x, -sampleImages[1]->GetHeight()*0.9, 0), positionAnimationDuration}
    }, AnimationBehaviour::Resetable);

    // High-pitched sound
    sampleImages[2]->LoadTexture(0, 0, "UI/Sesja/trumpetValve.png");
    sampleAnimators.push_back({
        GameObject::Instantiate("AlphaAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("PositionAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[2][0]->Setup(sampleImages[2], {
            {AnimatedProperty::Alpha, glm::vec3(0.6f), 0.2f},
    }, AnimationBehaviour::Resetable);
    sampleAnimators[2][1]->Setup(sampleImages[2], {
            {AnimatedProperty::Position, glm::vec3(valveInitPos[2].x, -sampleImages[2]->GetHeight()*0.9, 0), positionAnimationDuration}
    }, AnimationBehaviour::Resetable);

    for (int i = 0; i < 3; ++i) {
        sampleImages[i]->pivot = {0.5, 0};
        sampleImages[i]->SetPosition(valveInitPos[i].x, valveInitPos[i].y);
    }
}

void TrumpetSessionUI::Update() {
    float gain;
    for (int i=0; i<sampleSources.size(); ++i) {
        if(!soundsToFade[i]) continue;

        gain = sampleSources[i]->GetGain();
        sampleSources[i]->SetGain(gain-0.05f);
        if (gain <= 0.05f) {
            sampleSources[i]->SetGain(0);
            soundsToFade[i] = false;
            continue;
        }
    }

    SessionUI::Update();
}

void TrumpetSessionUI::PlaySound(int index) {
    for (int i = 0; i < sampleAnimators[index].size(); ++i) {
        sampleAnimators[index][i]->paused = false;
        sampleAnimators[index][i]->Reset();
    }

    soundsToFade[index] = false;
    sampleSources[index]->StopSound();
    sampleSources[index]->SetGain(1.0f);

    SessionUI::PlaySound(index);
}

void TrumpetSessionUI::StopSound(int index) {
    for (int i = 0; i < sampleAnimators[index].size(); ++i) {
        sampleAnimators[index][i]->paused = true;
    }
    sampleImages[index]->SetPosition(valveInitPos[index].x, valveInitPos[index].y);
    sampleImages[index]->SetAlpha(1);

    soundsToFade[index] = true;
    SessionUI::StopSound(index);
}
