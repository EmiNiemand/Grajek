#include "Components/Scripts/SessionUI/ClapSessionUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Button.h"
#include "Components/Animations/UIAnimator.h"

ClapSessionUI::ClapSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void ClapSessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples,
                          bool sessionMetronomeSound, bool sessionMetronomeVisuals, bool sessionBackingTrack) {
    // Load backing track
    // ------------------
    BackingTrackSetup("clap/backingTrack");

    SessionUI::Setup(bpm, samples, sessionMetronomeSound, sessionMetronomeVisuals, sessionBackingTrack);

    // Set up cheat sheet
    // ------------------
    SetCheatSheet("UI/Sesja/clapPatterns.png");

    // Set up instrument control
    // ------------------
    SetInstrumentControl("UI/Sesja/clapControl.png");

    // Load theme
    // ----------
    GameObject::Instantiate("Theme", parent)->AddComponent<Image>()
     ->LoadTexture(0, 0, "UI/Sesja/widokKlaskanie.png");


    // Set up samples
    // --------------
    sampleImages[0]->LoadTexture(1000, 300, "UI/Sesja/ClapLeft.png");
    sampleImages[1]->LoadTexture(1560, 300, "UI/Sesja/ClapRight.png");
    // Clap
    // ----
    sampleAnimators.push_back({
        GameObject::Instantiate("ScaleLeftAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("ScaleRightAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("PositionLeftAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("PositionRightAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[0][0]->Setup(sampleImages[0], {
        {AnimatedProperty::Scale, glm::vec3(1.5f), 0},
        {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][1]->Setup(sampleImages[1], {
            {AnimatedProperty::Scale, glm::vec3(1.5f), 0},
            {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][2]->Setup(sampleImages[0], {
            {AnimatedProperty::Position, glm::vec3(1000 + sampleImages[0]->GetWidth(), 300, 0), 0},
            {AnimatedProperty::Position, glm::vec3(1000, 300, 0), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][3]->Setup(sampleImages[1], {
            {AnimatedProperty::Position, glm::vec3(1560 - sampleImages[0]->GetWidth(), 300, 0), 0},
            {AnimatedProperty::Position, glm::vec3(1560, 300, 0), 0.2f}
    }, AnimationBehaviour::Resetable);
    // Stomp
    // -----
    sampleAnimators.push_back({
            GameObject::Instantiate("ScaleLeftAnimator", parent)->AddComponent<UIAnimator>(),
            GameObject::Instantiate("ScaleRightAnimator", parent)->AddComponent<UIAnimator>(),
            GameObject::Instantiate("PositionLeftAnimator", parent)->AddComponent<UIAnimator>(),
            GameObject::Instantiate("PositionRightAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[1][0]->Setup(sampleImages[0], {
            {AnimatedProperty::Scale, glm::vec3(1.5f), 0},
            {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][1]->Setup(sampleImages[1], {
            {AnimatedProperty::Scale, glm::vec3(1.5f), 0},
            {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][2]->Setup(sampleImages[0], {
            {AnimatedProperty::Position, glm::vec3(1000, 0, 0), 0},
            {AnimatedProperty::Position, glm::vec3(1000, 300, 0), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][3]->Setup(sampleImages[1], {
            {AnimatedProperty::Position, glm::vec3(1560, 0, 0), 0},
            {AnimatedProperty::Position, glm::vec3(1560, 300, 0), 0.2f}
    }, AnimationBehaviour::Resetable);

    // Add buttons
    int x = -485, y = 603;
    for (int i = 0; i < 2; i++, y -= 145) {
        soundButtons.push_back(GameObject::Instantiate("clapPatternsButton", parent)->AddComponent<Button>());
        soundButtons[i]->LoadTexture(x, y, "UI/Sesja/clapPatternsInactive.png", "UI/Sesja/clapPatternsSelect.png", -0.85);
        soundButtons[i]->isActive = false;
        patternsSounds.push_back(GameObject::Instantiate("clapPatternsSound", parent)->AddComponent<AudioSource>());
        patternsSounds[i]->LoadAudioData("res/sounds/direct/clap/pattern" + std::to_string(i + 1) + ".wav", AudioType::Direct);
        soundAnimators.push_back({GameObject::Instantiate("clapPatternsButtonAnimator", parent)->AddComponent<UIAnimator>(), GameObject::Instantiate("clapPatternsButtonAnimator", parent)->AddComponent<UIAnimator>()});
        soundAnimators[i][0]->Setup(soundButtons[i], {
                {AnimatedProperty::Position, glm::vec3(500, y, -0.85), 0.5f}
        }, AnimationBehaviour::Resetable);
        soundAnimators[i][1]->Setup(soundButtons[i], {
                {AnimatedProperty::Position, glm::vec3(x, y, -0.85), 0.5f}
        }, AnimationBehaviour::Resetable);
    }
    soundButtons[0]->up = soundButtons[1];
    soundButtons[0]->down = soundButtons[1];
    soundButtons[1]->up = soundButtons[0];
    soundButtons[1]->down = soundButtons[0];
}

void ClapSessionUI::PlaySound(int index) {
    for (int i = 0; i < sampleAnimators[index].size(); ++i) {
        sampleAnimators[index][i]->Reset();
    }
    SessionUI::PlaySound(index);
}