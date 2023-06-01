#include "Components/Scripts/Opponent/OpponentMusicSession.h"
#include "Components/Scripts/MusicPattern.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/SessionUI/ClapSessionUI.h"
#include "Components/Scripts/SessionUI/DrumsSessionUI.h"
#include "Components/Scripts/SessionUI/TrumpetSessionUI.h"
#include "Components/Scripts/SessionUI/LaunchpadSessionUI.h"
#include "Components/Scripts/SessionUI/GuitarSessionUI.h"
#include "EngineManagers/AIManager.h"
#include "Components/Scripts/Opponent/Opponent.h"

OpponentMusicSession::OpponentMusicSession(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void OpponentMusicSession::Setup(std::shared_ptr<Instrument> playerInstrument) {
    instrument = std::move(playerInstrument);

    bpm = (int)instrument->genre;

    auto sessionUIInstance = GameObject::Instantiate("OpponentSessionUI", parent);

    GameObject::Instantiate("Background", sessionUIInstance)
            ->AddComponent<Image>()->LoadTexture(0, 0, "UI/backgroundOpacity60.png", 0.8);

    switch (instrument->name) {
        case Clap:      sessionUI = sessionUIInstance->AddComponent<ClapSessionUI>(); break;
        case Drums:     sessionUI = sessionUIInstance->AddComponent<DrumsSessionUI>(); break;
        case Trumpet:   sessionUI = sessionUIInstance->AddComponent<TrumpetSessionUI>(); break;
        case Launchpad: sessionUI = sessionUIInstance->AddComponent<LaunchpadSessionUI>(); break;
        case Guitar:    sessionUI = sessionUIInstance->AddComponent<GuitarSessionUI>(); break;
    }
    sessionUI->Setup(bpm, instrument->samples, "UI/Sesja/Ramka.png");
    parent->parent->GetComponent<Opponent>()->sessionUI = sessionUI;
}

void OpponentMusicSession::Update() {
    timeoutCounter += GloomEngine::GetInstance()->deltaTime;

    if (!recordedSounds.empty() && timeoutCounter > timeout)
        PatternTimeout();
    Component::Update();
}

void OpponentMusicSession::PlaySample(int index) {
    if(instrument->samples.empty() || instrument->samples.size()-1 < index) return;

    sessionUI->PlaySound(index);

    timeoutCounter = 0;

    auto currentTime = (float)glfwGetTime();
    float rhythmDiff = GetRhythmValue(currentTime - lastTime);

    recordedSounds.emplace_back(instrument->samples[index], rhythmDiff, currentTime);
    lastTime = currentTime;

    DetectPattern();
}

void OpponentMusicSession::StopSample(int index) {
    if(instrument->samples.empty() || instrument->samples.size()-1 < index) return;
    if(recordedSounds.empty()) return;

    for (auto it = recordedSounds.rbegin(); it != recordedSounds.rend(); ++it) {
        if(it->sample->id == index) {
            it->duration = GetRhythmValue((float)glfwGetTime() - it->duration);
        }
    }

    // TODO: implement hold-type sounds
}

void OpponentMusicSession::ToggleCheatSheet() {
    sessionUI->ToggleCheatSheet();
}

void OpponentMusicSession::ToggleInstrumentControl() {
    sessionUI->ToggleInstrumentControl();
}

void OpponentMusicSession::DetectPattern() {
// Initialize potential patterns list
    // ----------------------------------
    if(potentialPatterns.empty())
    {
        auto lastItem = recordedSounds.back();
        recordedSounds.clear();
        recordedSounds.push_back(lastItem);

        for (const auto& pattern : instrument->patterns)
        {
            if(recordedSounds[0].sample->id == pattern->sounds[0]->sample->id)
                potentialPatterns.push_back(pattern);
        }
        return;
    }

    // Remove patterns that don't match anymore
    // ----------------------------------------
    int lastIndex = (int)recordedSounds.size() - 1;

    for (auto pattern=potentialPatterns.begin(); pattern!=potentialPatterns.end(); )
    {
        if(pattern->get()->sounds[lastIndex]->sample->id != recordedSounds[lastIndex].sample->id)
            pattern = potentialPatterns.erase(pattern);
        else
            ++pattern;
    }

    // User played sequence that doesn't match any patterns
    // ----------------------------------------------------
    if (potentialPatterns.empty())
    {
        spdlog::info("No patterns match");
        PatternFail();
        return;
    }
    // User played correct pattern ^^
    // ------------------------------
    for(const auto& pattern : potentialPatterns)
    {
        if(pattern->sounds.size() == recordedSounds.size())
            CalcAccuracyAndReset(pattern);
    }
}

void OpponentMusicSession::CalcAccuracyAndReset(const std::shared_ptr<MusicPattern> &goodPattern) {
    float accuracy = 0;
    for (int i = 0; i < recordedSounds.size(); i++)
    {
        float recordedDelay = i == 0 ? 0 : recordedSounds[i].delay;
        accuracy += abs(goodPattern->sounds[i]->delay - recordedDelay);
    }

    accuracy /= (float)recordedSounds.size();
    spdlog::info("[MS] Accuracy: " + std::to_string((1 - accuracy) * 100) + "%");
    PatternSuccess(goodPattern, 1 - accuracy);

    recordedSounds.clear();
    potentialPatterns.clear();
}

void OpponentMusicSession::PatternSuccess(const std::shared_ptr<MusicPattern>& pattern, float accuracy) {
    AIManager::GetInstance()->NotifyPlayerPlayedPattern(pattern);
    parent->parent->GetComponent<Opponent>()->UpdateSatisfaction(AIManager::GetInstance()->GetCombinedPlayerSatisfaction());
    sessionUI->UpdateAccuracy(accuracy);
    //TODO: play some kind of success sound
    // sessionUI.DiscoveredPattern();
}

void OpponentMusicSession::PatternTimeout() {
    recordedSounds.clear();
    potentialPatterns.clear();
    AIManager::GetInstance()->NotifyPlayerPlayedPattern(nullptr);
    parent->parent->GetComponent<Opponent>()->UpdateSatisfaction(AIManager::GetInstance()->GetCombinedPlayerSatisfaction());
    //TODO: play timeout sound
}

void OpponentMusicSession::PatternFail() {
    sessionUI->UpdateAccuracy(0);
    AIManager::GetInstance()->NotifyPlayerPlayedPattern(nullptr);
    parent->parent->GetComponent<Opponent>()->UpdateSatisfaction(AIManager::GetInstance()->GetCombinedPlayerSatisfaction());
    //TODO: play some kind of failure sound (record scratch)
}

float OpponentMusicSession::GetRhythmValue(float currentNoteLength) const {
    return currentNoteLength * ((float)bpm/60.0f);
}

void OpponentMusicSession::ToggleMetronomeVisuals() {

}

void OpponentMusicSession::ToggleMetronomeSound() {

}

void OpponentMusicSession::OnDestroy() {
    sessionUI.reset();
    potentialPatterns.clear();
    instrument.reset();
    Component::OnDestroy();
}