#include "Components/Scripts/SessionUI/TrumpetSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

TrumpetSessionUI::TrumpetSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void TrumpetSessionUI::Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, std::string metronomePath) {
    SessionUI::Setup(bpm, samples, metronomePath);

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
    // Medium-pitched sound
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
    // Loud-pitched sound
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

void TrumpetSessionUI::PlaySound(int index) {
    // TODO to do
    SessionUI::PlaySound(index);
}