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

    // Set up samples
    // --------------
    // Hi-Hat
    sampleImages[0]->LoadTexture(950, 800, "UI/Sesja/circle.png");
    sampleAnimators.push_back({
        GameObject::Instantiate("ScaleAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("ColorAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[0][0]->Setup(sampleImages[0], {
            {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
            {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][1]->Setup(sampleImages[0], {
            {AnimatedProperty::Color, glm::vec3(0.0f, 0.0f, 1.0f), 0.125f},
            {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    // Kick
    sampleImages[1]->LoadTexture(1250, 800, "UI/Sesja/circle.png");
    sampleAnimators.push_back({
              GameObject::Instantiate("ScaleAnimator", parent)->AddComponent<UIAnimator>(),
              GameObject::Instantiate("ColorAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[1][0]->Setup(sampleImages[1], {
            {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
            {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][1]->Setup(sampleImages[1], {
            {AnimatedProperty::Color, glm::vec3(1.0f, 0.0f, 0.0f), 0.125f},
            {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    // Snare
    sampleImages[2]->LoadTexture(1550, 800, "UI/Sesja/circle.png");
    sampleAnimators.push_back({
          GameObject::Instantiate("ScaleAnimator", parent)->AddComponent<UIAnimator>(),
          GameObject::Instantiate("ColorAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[2][0]->Setup(sampleImages[2], {
        {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
        {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[2][1]->Setup(sampleImages[2], {
        {AnimatedProperty::Color, glm::vec3(0.0f, 0.0f, 1.0f), 0.125f},
        {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
    }, AnimationBehaviour::Resetable);
}

void DrumsSessionUI::PlaySound(int index) {
    // Set up animated notes
    // ---------------------
    auto animator = GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>();
    auto nuta = GameObject::Instantiate("Nuta", animator->GetParent())->AddComponent<Image>();
    auto generator = RandomnessManager::GetInstance();
    if (index == 0) {
        int random = generator->GetInt(500, 700);
        nuta->LoadTexture(random, 675, "UI/Sesja/Nuta1.png", -0.1f);
        animator->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(random, 750.0f, 0.0f)}
        });
    } else if (index == 1) {
        int random = generator->GetInt(150, 450);
        nuta->LoadTexture(625, random, "UI/Sesja/Nuta2.png", -0.1f);
        animator->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(700.0f, random, 0.0f)}
        });
    } else if (index == 2) {
        int random = generator->GetInt(250, 450);
        nuta->LoadTexture(random, 475, "UI/Sesja/Nuta3.png", -0.1f);
        animator->Setup(nuta, {
                {AnimatedProperty::Position, glm::vec3(random, 550.0f, 0.0f)}
        });
    }
    //GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(nuta, {
    //        {AnimatedProperty::Rotation,
    //            glm::vec3(0.0f, 0.0f, generator->GetFloat(-45.0f, 45.0f))}
    //});
    //GameObject::Instantiate("NutaAnimator", parent->parent)->AddComponent<UIAnimator>()->Setup(nuta, {
    //        {AnimatedProperty::Scale, glm::vec3(0.5f)}
    //});
    nuta->SetScale(generator->GetFloat(1, 3));

    for (int i = 0; i < sampleAnimators[index].size(); ++i) {
        sampleAnimators[index][i]->Reset();
    }
    SessionUI::PlaySound(index);
}