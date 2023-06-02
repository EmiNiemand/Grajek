#include "Components/Scripts/SessionUI/DrumsSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"
#include "EngineManagers/RandomnessManager.h"

DrumsSessionUI::DrumsSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void DrumsSessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, std::string metronomePath) {
    SessionUI::Setup(bpm, samples, metronomePath);

    // Add cheat sheet
    // ------------------
    SetCheatSheet("UI/Sesja/drumPatterns.png");

    // Load theme
    // ----------
    GameObject::Instantiate("Theme", parent)->AddComponent<Image>()
            ->LoadTexture(0, 0, "UI/Sesja/widokPerkusja.png");

    // Load backing track
    // ------------------
    BackingTrackSetup("drums/backingTrack");

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