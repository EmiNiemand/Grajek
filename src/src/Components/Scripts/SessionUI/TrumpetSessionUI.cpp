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
    BackingTrackSetup("drums/backingTrack");

    SessionUI::Setup(bpm, samples, sessionMetronomeSound, sessionMetronomeVisuals, sessionBackingTrack);

    // Add cheat sheet
    // ------------------
    SetCheatSheet("UI/Sesja/drumPatterns.png");

    // Load theme
    // ----------
    GameObject::Instantiate("Theme", parent)->AddComponent<Image>()
            ->LoadTexture(0, 0, "UI/Sesja/widokTrabka.png");

    // Set up samples
    // --------------
    // Low-pitched sound
    sampleImages[0]->LoadTexture(1000, 0, "UI/Sesja/trumpetValve.png");
    sampleAnimators.push_back({
                                      GameObject::Instantiate("ColorAnimator", parent)->AddComponent<UIAnimator>(),
                                      GameObject::Instantiate("PositionAnimator", parent)->AddComponent<UIAnimator>()
                              });
    sampleAnimators[0][0]->Setup(sampleImages[0], {
            {AnimatedProperty::Color, glm::vec3(0.0f, 0.0f, 1.0f), 0.125f},
            {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][1]->Setup(sampleImages[0], {
            {AnimatedProperty::Position, glm::vec3(950, 300, 0), 2}
    }, AnimationBehaviour::Resetable);
    // Medium-pitched sound
    sampleImages[1]->LoadTexture(1300, 0, "UI/Sesja/trumpetValve.png");
    sampleImages[1]->SetPosition(1300, sampleImages[1]->GetHeight()/2);
    sampleAnimators.push_back({
        GameObject::Instantiate("ColorAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("PositionAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[1][0]->Setup(sampleImages[1], {
            {AnimatedProperty::Color, glm::vec3(1.0f, 0.0f, 0.0f), 0.125f},
            {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][1]->Setup(sampleImages[1], {
            {AnimatedProperty::Position, glm::vec3(1250, 300, 0), 2}
    }, AnimationBehaviour::Resetable);
    // High-pitched sound
    sampleImages[2]->LoadTexture(1600, 0, "UI/Sesja/trumpetValve.png");
    sampleImages[2]->SetPosition(1600, sampleImages[2]->GetHeight()/2);
    sampleAnimators.push_back({
                                      GameObject::Instantiate("ColorAnimator", parent)->AddComponent<UIAnimator>(),
                                      GameObject::Instantiate("PositionAnimator", parent)->AddComponent<UIAnimator>()
                              });
    sampleAnimators[2][0]->Setup(sampleImages[2], {
            {AnimatedProperty::Color, glm::vec3(0.0f, 0.0f, 1.0f), 0.125f},
            {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[2][1]->Setup(sampleImages[2], {
            {AnimatedProperty::Position, glm::vec3(1550, 300, 0), 2}
    }, AnimationBehaviour::Resetable);
}

void TrumpetSessionUI::Update() {
    for (int i=0; i<sampleSources.size(); ++i) {
        if(!soundsToFade[i]) continue;

        float gain = sampleSources[i]->GetGain();
        sampleSources[i]->SetGain(gain-0.05f);
        if(gain <= 0.1f) { sampleSources[i]->SetGain(0); soundsToFade[i] = false; continue; }
    }

    SessionUI::Update();
}

void TrumpetSessionUI::PlaySound(int index) {
    sampleAnimators[index][2]->paused = false;
    for (int i = 0; i < sampleAnimators[index].size(); ++i) {
        sampleAnimators[index][i]->Reset();
    }

    soundsToFade[index] = false;
    sampleSources[index]->SetGain(1.0f);

    SessionUI::PlaySound(index);
}

void TrumpetSessionUI::StopSound(int index) {
    sampleAnimators[index][2]->paused = true;
    glm::vec2 position;
    //TODO: move positions to some variable in class
    if(index == 0) position = {950, 800};
    if(index == 1) position = {1250, 800};
    if(index == 2) position = {1550, 800};
    sampleImages[index]->SetPosition(position.x, position.y);

    soundsToFade[index] = true;
    SessionUI::StopSound(index);
}
