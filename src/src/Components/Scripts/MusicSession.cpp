//
// Created by masterktos on 06.04.23.
//

#include "Components/Scripts/MusicSession.h"
#include "Components/Scripts/MusicPattern.h"
#include "Components/Scripts/PlayerManager.h"
#include "Components/Scripts/SessionUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

MusicSession::MusicSession(const std::shared_ptr<GameObject> &parent, uint32_t id) : Component(parent, id) {}

void MusicSession::Setup(std::shared_ptr<Instrument> playerInstrument) {
    instrument = std::move(playerInstrument);

    bpm = (int)instrument->genre;

    playerManager = parent->GetComponent<PlayerManager>();

    sessionUI = GameObject::Instantiate("Session", GloomEngine::GetInstance()->FindGameObjectWithName("SessionUI"))->AddComponent<SessionUI>();
    sessionUI->Setup(bpm, instrument->samples, nullptr);

    std::shared_ptr<GameObject> theme = GameObject::Instantiate("Theme", GloomEngine::GetInstance()->FindGameObjectWithName("SessionUI"));
    if (instrument->NameToString() == "Drums") {
        theme->AddComponent<Image>()->LoadTexture(0, 0, "UI/Sesja/widokPerkusja.png");
    } else if (instrument->NameToString() == "Trumpet") {
        theme->AddComponent<Image>()->LoadTexture(0, 0, "UI/Sesja/widokTrabka.png");
    } else if (instrument->NameToString() == "Launchpad") {
        theme->AddComponent<Image>()->LoadTexture(0, 0, "UI/Sesja/widokLaunchpad.png");
    } else if (instrument->NameToString() == "Guitar") {
        theme->AddComponent<Image>()->LoadTexture(0, 0, "UI/Sesja/widokGitara.png");
    }

    if (instrument->NameToString() == "Clap") {
        GameObject::Instantiate("Circle1", GloomEngine::GetInstance()->FindGameObjectWithName("SessionUI"))
        ->AddComponent<Image>()->LoadTexture(650, 600, "UI/Sesja/circle.png");
        GameObject::Instantiate("Circle2", GloomEngine::GetInstance()->FindGameObjectWithName("SessionUI"))
        ->AddComponent<Image>()->LoadTexture(1150, 600, "UI/Sesja/circle.png");
    } else if (instrument->NameToString() == "Drums") {
        GameObject::Instantiate("Circle1", GloomEngine::GetInstance()->FindGameObjectWithName("SessionUI"))
                ->AddComponent<Image>()->LoadTexture(950, 800, "UI/Sesja/circle.png");
        GameObject::Instantiate("Circle2", GloomEngine::GetInstance()->FindGameObjectWithName("SessionUI"))
                ->AddComponent<Image>()->LoadTexture(1250, 800, "UI/Sesja/circle.png");
        GameObject::Instantiate("Circle3", GloomEngine::GetInstance()->FindGameObjectWithName("SessionUI"))
                ->AddComponent<Image>()->LoadTexture(1550, 800, "UI/Sesja/circle.png");
    }
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

    DetectPattern();
}

void MusicSession::StopSample(int index) {
    if(instrument->samples.empty() || instrument->samples.size()-1 < index) return;
    if(recordedSounds.empty()) return;

    for (auto it = recordedSounds.rbegin(); it != recordedSounds.rend(); ++it) {
        if(it->sample->id == index) {
            it->duration = GetRhythmValue(glfwGetTime() - it->duration);
        }
    }

    // TODO: implement hold-type sounds
}

void MusicSession::ToggleCheatSheet() { sessionUI->ToggleCheatSheet(); }

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

void MusicSession::CalcAccuracyAndReset(const std::shared_ptr<MusicPattern> &goodPattern) {
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

void MusicSession::PatternSuccess(std::shared_ptr<MusicPattern> pattern, float accuracy) {
    playerManager->PlayedPattern(pattern);
    //TODO: play some kind of success sound
    // sessionUI.DiscoveredPattern();

    sessionUI->UpdateAccuracy(accuracy);
}

void MusicSession::PatternTimeout() {
    recordedSounds.clear();
    potentialPatterns.clear();

    playerManager->PlayedPattern(nullptr);
    //TODO: play timeout sound
}

void MusicSession::PatternFail() {
    sessionUI->UpdateAccuracy(0);
    playerManager->PlayedPattern(nullptr);
    //TODO: play some kind of failure sound (record scratch)
}

float MusicSession::GetRhythmValue(float currentNoteLength) { return currentNoteLength * (bpm/60.0f); }

void MusicSession::Stop() {
    parent->RemoveComponent(GetId());
    sessionUI->GetParent()->parent->RemoveAllChildren();
}
