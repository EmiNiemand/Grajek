#include "Components/Scripts/SessionUI/DrumsSessionUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Button.h"
#include "Components/Animations/UIAnimator.h"
#include "EngineManagers/RandomnessManager.h"

DrumsSessionUI::DrumsSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void DrumsSessionUI::Setup(std::shared_ptr<Instrument> instrument, bool sessionMetronomeSound,
                           bool sessionMetronomeVisuals, bool sessionBackingTrack) {
    SessionUI::Setup(instrument, sessionMetronomeSound, sessionMetronomeVisuals, sessionBackingTrack);

    // Set up samples
    // --------------
    // Hi-Hat
    sampleImages[0]->LoadTexture(1000, 320, "UI/Sesja/DrumsHiHat.png");
    sampleAnimators.push_back({
        GameObject::Instantiate("ScaleAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("AlphaAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[0][0]->Setup(sampleImages[0], {
            {AnimatedProperty::Scale, glm::vec3(1.5f), 0.05f},
            {AnimatedProperty::Scale, glm::vec3(1.1f), 0.05f},
            {AnimatedProperty::Scale, glm::vec3(1.3f), 0.05f},
            {AnimatedProperty::Scale, glm::vec3(1.0f), 0.05f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][1]->Setup(sampleImages[0], {
            {AnimatedProperty::Alpha, glm::vec3(0.6f), 0.1f},
            {AnimatedProperty::Alpha, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    // Kick
    sampleImages[1]->LoadTexture(1250, 230, "UI/Sesja/DrumsKick.png");
    sampleAnimators.push_back({
              GameObject::Instantiate("ScaleAnimator", parent)->AddComponent<UIAnimator>(),
              GameObject::Instantiate("AlphaAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[1][0]->Setup(sampleImages[1], {
            {AnimatedProperty::Scale, glm::vec3(2.0f), 0.05f},
            {AnimatedProperty::Scale, glm::vec3(1.0f), 0.20f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][1]->Setup(sampleImages[1], {
            {AnimatedProperty::Alpha, glm::vec3(0.6f), 0.1f},
            {AnimatedProperty::Alpha, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    // Snare
    sampleImages[2]->LoadTexture(1500, 320, "UI/Sesja/DrumsSnare.png");
    sampleAnimators.push_back({
          GameObject::Instantiate("ScaleAnimator", parent)->AddComponent<UIAnimator>(),
          GameObject::Instantiate("AlphaAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[2][0]->Setup(sampleImages[2], {
        {AnimatedProperty::Scale, glm::vec3(1.5f), 0.05f},
        {AnimatedProperty::Scale, glm::vec3(1.0f), 0.20f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[2][1]->Setup(sampleImages[2], {
        {AnimatedProperty::Alpha, glm::vec3(0.6f), 0.1f},
        {AnimatedProperty::Alpha, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);

    // Add buttons
    int x = 1600, y = 625;
    for (int i = 0; i < 4; i++, y -= 187) {
        soundButtons.push_back(GameObject::Instantiate("drumsPatternsButton", parent)->AddComponent<Button>());
        soundButtons[i]->LoadTexture(x+900, y, "UI/Sesja/clapPatternsInactive.png", "UI/Sesja/clapPatternsSelect.png", -0.85);
        soundButtons[i]->isActive = false;
        patternsSounds.push_back(GameObject::Instantiate("drumsPatternsSound", parent)->AddComponent<AudioSource>());
        patternsSounds[i]->LoadAudioData("res/sounds/direct/drums/pattern" + std::to_string(i + 1) + ".wav", AudioType::Direct);
        soundAnimators.push_back({GameObject::Instantiate("drumsPatternsButtonAnimator", parent)->AddComponent<UIAnimator>(), GameObject::Instantiate("drumsPatternsButtonAnimator", parent)->AddComponent<UIAnimator>()});
        soundAnimators[i][0]->Setup(soundButtons[i], {
                {AnimatedProperty::Position, glm::vec3(x, y, -0.85), 0.5}
        }, AnimationBehaviour::Resetable);
        soundAnimators[i][1]->Setup(soundButtons[i], {
                {AnimatedProperty::Position, glm::vec3(x+900, y, -0.85), 0.5}
        }, AnimationBehaviour::Resetable);
    }
    soundButtons[0]->up = soundButtons[3];
    soundButtons[0]->down = soundButtons[1];
    soundButtons[1]->up = soundButtons[0];
    soundButtons[1]->down = soundButtons[2];
    soundButtons[2]->up = soundButtons[1];
    soundButtons[2]->down = soundButtons[3];
    soundButtons[3]->up = soundButtons[2];
    soundButtons[3]->down = soundButtons[0];
}

void DrumsSessionUI::PlaySound(int index) {
    // Set up animated notes
    // ---------------------
    auto animator = GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>();
    auto nuta = GameObject::Instantiate("Nuta", animator->GetParent())->AddComponent<Image>();
    auto generator = RandomnessManager::GetInstance();
    if (index == 0) {
        int random = generator->GetInt(430, 630);
        nuta->LoadTexture(random, 575, "UI/Sesja/Nuta1.png", -0.1f);
        animator->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(random, 750.0f, 0.0f)}
        });
    } else if (index == 1) {
        int random = generator->GetInt(120, 400);
        nuta->LoadTexture(525, random, "UI/Sesja/Nuta2.png", -0.1f);
        animator->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(700.0f, random, 0.0f)}
        });
    } else if (index == 2) {
        int random = generator->GetInt(210, 360);
        nuta->LoadTexture(random, 390, "UI/Sesja/Nuta3.png", -0.1f);
        animator->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(random, 550.0f, 0.0f)}
        });
    }
    nuta->SetScale(generator->GetFloat(1, 2));

    for (int i = 0; i < sampleAnimators[index].size(); ++i) {
        sampleAnimators[index][i]->Reset();
    }
    SessionUI::PlaySound(index);
}