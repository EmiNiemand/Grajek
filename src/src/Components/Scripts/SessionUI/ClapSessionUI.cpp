#include "Components/Scripts/SessionUI/ClapSessionUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Button.h"
#include "Components/Animations/UIAnimator.h"

ClapSessionUI::ClapSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void ClapSessionUI::Setup(std::shared_ptr<Instrument> instrument) {
    SessionUI::Setup(instrument);

    // Set up samples
    // --------------
    int xPos0 = 700, xPos1 = 1060, yPos = 300;
    sampleImages[0]->LoadTexture(xPos0, yPos, "UI/Sesja/ClapLeft.png");
    sampleImages[1]->LoadTexture(xPos1, yPos, "UI/Sesja/ClapRight.png");
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
            {AnimatedProperty::Position, glm::vec3(xPos0 + sampleImages[0]->GetWidth(), yPos, 0), 0},
            {AnimatedProperty::Position, glm::vec3(xPos0, yPos, 0), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][3]->Setup(sampleImages[1], {
            {AnimatedProperty::Position, glm::vec3(xPos1 - sampleImages[0]->GetWidth(), yPos, 0), 0},
            {AnimatedProperty::Position, glm::vec3(xPos1, yPos, 0), 0.2f}
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
            {AnimatedProperty::Position, glm::vec3(xPos0, 0, 0), 0},
            {AnimatedProperty::Position, glm::vec3(xPos0, yPos, 0), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][3]->Setup(sampleImages[1], {
            {AnimatedProperty::Position, glm::vec3(xPos1, 0, 0), 0},
            {AnimatedProperty::Position, glm::vec3(xPos1, yPos, 0), 0.2f}
    }, AnimationBehaviour::Resetable);

    // Add buttons
    int x = 1625, y = 625;
    for (int i = 0; i < 2; i++, y -= 105) {
        soundButtons.push_back(GameObject::Instantiate("clapPatternsButton", parent)->AddComponent<Button>());
        soundButtons[i]->LoadTexture(x+700, y, "UI/Sesja/clapPatternsInactive.png", "UI/Sesja/clapPatternsSelect.png", -0.85);
        soundButtons[i]->isActive = false;
        soundButtons[i]->SetScale(0.8);
        patternsSounds.push_back(GameObject::Instantiate("clapPatternsSound", parent)->AddComponent<AudioSource>());
        patternsSounds[i]->LoadAudioData("res/sounds/direct/clap/pattern" + std::to_string(i + 1) + ".wav", AudioType::Direct);
        soundAnimators.push_back({GameObject::Instantiate("clapPatternsButtonAnimator", parent)->AddComponent<UIAnimator>(),
                                  GameObject::Instantiate("clapPatternsButtonAnimator", parent)->AddComponent<UIAnimator>()});
        soundAnimators[i][0]->Setup(soundButtons[i], {
                {AnimatedProperty::Position, glm::vec3(x, y, -0.85), 0.5f}
        }, AnimationBehaviour::Resetable);
        soundAnimators[i][1]->Setup(soundButtons[i], {
                {AnimatedProperty::Position, glm::vec3(x+700, y, -0.85), 0.5f}
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