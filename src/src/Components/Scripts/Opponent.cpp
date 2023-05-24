#include "GloomEngine.h"
#include "Components/Scripts/Opponent.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/OpponentManager.h"
#include "Components/Scripts/Instrument.h"

Opponent::Opponent(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Opponent::~Opponent() = default;

void Opponent::Setup(std::shared_ptr<Instrument> instrument1, std::vector<RawSample> musicPattern, float accuracy1) {
    instrument = std::move(instrument1);
    instrument->GeneratePattern(musicPattern);
    pattern = instrument->patterns.back();
    pattern->sounds[0]->delay = musicPattern[0].delay;
    for (const auto& sound: instrument->patterns.back()->sounds) {
        sampleSources.push_back(GameObject::Instantiate("OpponentSampleSource", parent)->AddComponent<AudioSource>());
        auto sample = sampleSources.back();
        sample->LoadAudioData(sound->sample->clipPath.c_str(), AudioType::Positional);
        sample->SetPositionOffset(parent->parent->transform->GetLocalPosition());
    }
    accuracy = accuracy1;
}

void Opponent::Update() {
    if (defeated) return;

    timer += GloomEngine::GetInstance()->deltaTime;
    if (timer >= pattern->sounds[sampleIndex]->delay) {
        timer = 0.0f;
        sampleSources[sampleIndex]->ForcePlaySound();
        sampleIndex++;
        if (sampleIndex >= pattern->sounds.size())
            sampleIndex = 0;
    }

    if (wellPlayedPatternCount >= 10) {
        defeated = true;
    }

    Component::Update();
}

void Opponent::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
    Component::OnTriggerEnter(gameObject);
}

void Opponent::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = false;
    Component::OnTriggerExit(gameObject);
}

void Opponent::UpdateSatisfaction(float satisfaction) {
    if (triggerActive) {
        if (satisfaction >= accuracy) wellPlayedPatternCount++;
        if (satisfactionAverage == 0.0f) {
            satisfactionAverage = satisfaction;
            return;
        }
        satisfactionAverage += satisfaction;
        satisfactionAverage /= 2;
    }
}

void Opponent::OnCreate() {
    OpponentManager::GetInstance()->opponents.push_back(std::dynamic_pointer_cast<Opponent>(shared_from_this()));
    Component::OnCreate();
}

void Opponent::OnDestroy() {
    std::remove(OpponentManager::GetInstance()->opponents.begin(), OpponentManager::GetInstance()->opponents.end(), shared_from_this());
    Component::OnDestroy();
}