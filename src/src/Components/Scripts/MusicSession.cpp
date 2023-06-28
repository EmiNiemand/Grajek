//
// Created by masterktos on 06.04.23.
//

#include "Components/Scripts/MusicSession.h"
#include "Components/Scripts/MusicPattern.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/SessionUI/ClapSessionUI.h"
#include "Components/Scripts/SessionUI/DrumsSessionUI.h"
#include "Components/Scripts/SessionUI/TrumpetSessionUI.h"
#include "Components/Scripts/SessionUI/LaunchpadSessionUI.h"
#include "Components/Scripts/SessionUI/GuitarSessionUI.h"
#include "Components/Audio/AudioSource.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

MusicSession::MusicSession(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void MusicSession::Setup(std::shared_ptr<Instrument> playerInstrument) {
    instrument = std::move(playerInstrument);

    bpm = (int)instrument->genre;

    playerManager = parent->parent->GetComponent<PlayerManager>();

    auto sessionUIInstance = GloomEngine::GetInstance()->FindGameObjectWithName("SessionUI");
    sessionUIInstance = GameObject::Instantiate("Session", sessionUIInstance);

    auto patternFailureAudioSource = GameObject::Instantiate("FailureAudioSource", parent);
    patternFailureSound = patternFailureAudioSource->AddComponent<AudioSource>();
    patternFailureSound->LoadAudioData("res/sounds/direct/on_pattern_failure.wav", AudioType::Direct);

    auto patternTimeoutAudioSource = GameObject::Instantiate("TimeoutAudioSource", parent);
    patternTimeoutSound = patternTimeoutAudioSource->AddComponent<AudioSource>();
    patternTimeoutSound->LoadAudioData("res/sounds/direct/on_pattern_timeout.wav", AudioType::Direct);

    switch (instrument->name) {
        case Clap:      sessionUI = sessionUIInstance->AddComponent<ClapSessionUI>(); break;
        case Drums:     sessionUI = sessionUIInstance->AddComponent<DrumsSessionUI>(); break;
        case Trumpet:   sessionUI = sessionUIInstance->AddComponent<TrumpetSessionUI>(); break;
        case Launchpad: sessionUI = sessionUIInstance->AddComponent<LaunchpadSessionUI>(); break;
        case Guitar:    sessionUI = sessionUIInstance->AddComponent<GuitarSessionUI>(); break;
    }
    sessionUI->Setup(instrument);
}

void MusicSession::Update() {
#ifdef DEBUG
    ZoneScopedNC("Music session", 0x800080);
#endif
    timeoutCounter += GloomEngine::GetInstance()->deltaTime;

    if (!recordedSounds.empty() && timeoutCounter > timeout)
        PatternTimeout();
}

void MusicSession::PlaySample(int index) {
    if(instrument->samples.empty() || instrument->samples.size()-1 < index) return;

    sessionUI->PlaySound(index);

    timeoutCounter = 0;

    float currentTime = glfwGetTime();
    float rhythmDiff = GetRhythmValue(currentTime - lastTime);

    recordedSounds.emplace_back(instrument->samples[index], rhythmDiff, currentTime);
    lastTime = currentTime;

    if(instrument->name == InstrumentName::Clap || instrument->name == InstrumentName::Drums)
        DetectPattern();
}

void MusicSession::StopSample(int index) {
    if(instrument->samples.empty() || instrument->samples.size()-1 < index) return;
    // Some samples might be left over from previously played pattern
    sessionUI->StopSound(index);

    if(recordedSounds.empty()) return;

    lastTime = glfwGetTime();

    for (auto it = recordedSounds.rbegin(); it != recordedSounds.rend(); ++it) {
        if(it->sample->id == index) {
            it->duration = GetRhythmValue(glfwGetTime() - it->duration);
            break;
        }
    }

    if(!(instrument->name == InstrumentName::Clap || instrument->name == InstrumentName::Drums)) {
        DetectPattern();
    }
}

void MusicSession::ToggleCheatSheet() { sessionUI->ToggleCheatSheet(); }

void MusicSession::ToggleInstrumentControl() { sessionUI->ToggleInstrumentControl(); }

void MusicSession::DetectPattern() {
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
    int lastIndex = recordedSounds.size() - 1;
    // Yep, finally found it, and commented it -Adrian
//    spdlog::info("ID:");
//    spdlog::info(recordedSounds[lastIndex].sample->id);


    for (auto pattern=potentialPatterns.begin(); pattern!=potentialPatterns.end(); )
    {
        if(pattern->get()->sounds.size() < lastIndex+1 ||
           pattern->get()->sounds[lastIndex]->sample->id != recordedSounds[lastIndex].sample->id)
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

void MusicSession::CalcAccuracyAndReset(const std::shared_ptr<MusicPattern> &goodPattern) {
    float accuracy = 0;
    recordedSounds[0].delay = 0;

    for (int i = 0; i < recordedSounds.size(); i++)
    {
        // TODO: this is a quick and bad "fix" that is intended to repair trumpet
        //       (might be caused by the fact that the last recorded sound is added
        //       to the vector but it didn't stop playing just yet; maybe add some buffor?)
        if(instrument->name == InstrumentName::Trumpet) {
            if (recordedSounds[i].duration > 3) recordedSounds[i].duration = 3;
            // Since there's no delay between sounds in all patterns, let's make it a bit easier for player
            // (and emphasize length of playing rather than racing fingers)
            if (recordedSounds[i].delay < 2) recordedSounds[i].delay = 0;
        }
        float soundAccuracy = abs(goodPattern->sounds[i]->delay - recordedSounds[i].delay);
        if(goodPattern->sounds[i]->duration != 0)
            soundAccuracy = (soundAccuracy + abs(goodPattern->sounds[i]->duration - recordedSounds[i].duration)) / 2.0f;
        accuracy += soundAccuracy;
    }

    // TODO: another quick fix
    float divider = recordedSounds.size() > 0 ? recordedSounds.size() : 1;
    if(accuracy < 0) accuracy = 0;

    accuracy /= divider;
    spdlog::info("[MS] Accuracy: " + std::to_string((1 - accuracy) * 100) + "%");
    PatternSuccess(goodPattern, 1 - accuracy);

    recordedSounds.clear();
    potentialPatterns.clear();
}

void MusicSession::PatternSuccess(std::shared_ptr<MusicPattern> pattern, float accuracy) {
    playerManager->PlayedPattern(pattern, accuracy);
    sessionUI->UpdateAccuracy(accuracy);
}

void MusicSession::PatternTimeout() {
    recordedSounds.clear();
    potentialPatterns.clear();
    playerManager->PlayedPattern(nullptr, 0);
    patternTimeoutSound->ForcePlaySound();
}

void MusicSession::PatternFail() {
    sessionUI->UpdateAccuracy(0);
    playerManager->PlayedPattern(nullptr, 0);
    patternFailureSound->ForcePlaySound();
}

float MusicSession::GetRhythmValue(float currentNoteLength) { return currentNoteLength * (bpm/60.0f); }

void MusicSession::Stop() {
    GameObject::Destroy(parent);
    sessionUI->GetParent()->parent->RemoveAllChildren();
}

// Pass-through functions
bool MusicSession::ToggleMetronomeVisuals() {
    return sessionUI->ToggleMetronomeVisuals();
}

bool MusicSession::ToggleMetronomeSound() {
    return sessionUI->ToggleMetronomeSound();
}

bool MusicSession::ToggleBackingTrack() {
    return sessionUI->ToggleBackingTrack();
}

void MusicSession::OnDestroy() {
    sessionUI.reset();
    playerManager.reset();
    potentialPatterns.clear();
    patternFailureSound.reset();
    patternTimeoutSound.reset();
    instrument.reset();
    Component::OnDestroy();
}

void MusicSession::ChangeActiveButton(glm::vec2 moveVector) {
    sessionUI->ChangeActiveButton(moveVector);
}

void MusicSession::OnClick() {
    sessionUI->OnClick();
}
