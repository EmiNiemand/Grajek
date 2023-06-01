#include "Components/Scripts/SessionUI/ClapSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

ClapSessionUI::ClapSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void ClapSessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, std::string metronomePath) {
    SessionUI::Setup(bpm, samples, metronomePath);

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
    // Stomp
    sampleImages[0]->LoadTexture(960 - 200, 800, "UI/Sesja/circle.png");
    sampleAnimators.push_back({
        GameObject::Instantiate("ScaleAnimator", parent)->AddComponent<UIAnimator>(),
        GameObject::Instantiate("ColorAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[0][0]->Setup(sampleImages[0], {
        {AnimatedProperty::Scale, glm::vec3(3.0f), 0},
        {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[0][1]->Setup(sampleImages[0], {
        {AnimatedProperty::Color, glm::vec3(0.0f, 0.0f, 1.0f), 0},
        {AnimatedProperty::Color, glm::vec3(1.0f), 0.2f}
    }, AnimationBehaviour::Resetable);
    // Clap
    sampleImages[1]->LoadTexture(960 + 200, 800, "UI/Sesja/circle.png");
    sampleAnimators.push_back({
            GameObject::Instantiate("ScaleAnimator", parent)->AddComponent<UIAnimator>(),
            GameObject::Instantiate("ColorAnimator", parent)->AddComponent<UIAnimator>()
    });
    sampleAnimators[1][0]->Setup(sampleImages[1], {
            {AnimatedProperty::Scale, glm::vec3(3.0f), 0},
            {AnimatedProperty::Scale, glm::vec3(1.0f), 0.1f}
    }, AnimationBehaviour::Resetable);
    sampleAnimators[1][1]->Setup(sampleImages[1], {
            {AnimatedProperty::Color, glm::vec3(1.0f, 0.0f, 0.0f), 0},
            {AnimatedProperty::Color, glm::vec3(1.0f), 0.1f}
    }, AnimationBehaviour::Resetable);
}

void ClapSessionUI::PlaySound(int index) {
    for (int i = 0; i < sampleAnimators[index].size(); ++i) {
        sampleAnimators[index][i]->Reset();
    }
    SessionUI::PlaySound(index);
}
