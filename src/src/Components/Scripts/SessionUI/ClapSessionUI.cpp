#include "Components/Scripts/SessionUI/ClapSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
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
}

void ClapSessionUI::PlaySound(int index) {
    for (int i = 0; i < sampleAnimators[index].size(); ++i) {
        sampleAnimators[index][i]->Reset();
    }
    SessionUI::PlaySound(index);
}
