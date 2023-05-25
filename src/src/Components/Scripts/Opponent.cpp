#include "GloomEngine.h"
#include "Components/Scripts/Opponent.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/OpponentManager.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/Scripts/Player/PlayerManager.h"

Opponent::Opponent(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Opponent::~Opponent() = default;

void Opponent::Setup(std::shared_ptr<Instrument> instrument1, std::vector<RawSample> musicPattern, float satisfaction1) {
    // Setup pattern
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
    satisfaction = satisfaction1;

    // Setup UI
    ui = GameObject::Instantiate("OpponentUI", parent);
    auto indicator = GameObject::Instantiate("OpponentSatisfaction", ui)->AddComponent<Image>();
    indicator->LoadTexture(700 + (int)satisfaction * 5, 1000, "UI/satysfakcjaPrzeciwnika.png");
    indicator->SetScale(0.5);
    auto belt1 = GameObject::Instantiate("OpponentAverageSatisfaction", ui);
    for (int i = 0; i < 20; i++) {
        belt.push_back(GameObject::Instantiate("OpponentAverageSatisfaction", belt1)->AddComponent<Image>());
        belt.back()->LoadTexture(700 + i * 25, 1000, "UI/satysfakcjaGracza.png");
        belt.back()->SetScale(0.5f);
    }
    ui->DisableSelfAndChildren();

    dialogue = GameObject::Instantiate("OpponentDialogue", parent)->AddComponent<Dialogue>();
    dialogue->forced = true;
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
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->OnPlayerLoseDuel();
        dialogue->ShowDialogue();
    }

    Component::Update();
}

void Opponent::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
    ui->EnableSelfAndChildren();
    for (const auto & i : belt) {
        i->enabled = false;
    }
    Component::OnTriggerEnter(gameObject);
}

void Opponent::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = false;
    ui->DisableSelfAndChildren();
    Component::OnTriggerExit(gameObject);
}

void Opponent::UpdateSatisfaction(float satisfaction1) {
    if (triggerActive) {
        for (const auto & i : belt) {
            i->enabled = false;
        }
        if (satisfaction1 >= satisfaction) wellPlayedPatternCount++;
        if (satisfactionAverage == 0.0f) {
            satisfactionAverage = satisfaction1;
            for (int i = 0; i < (int)satisfactionAverage / 5; i++) {
                belt[i]->enabled = true;
            }
            return;
        }
        satisfactionAverage += satisfaction1;
        satisfactionAverage /= 2;
        for (int i = 0; i < (int)satisfactionAverage / 5; i++) {
            belt[i]->enabled = true;
        }
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