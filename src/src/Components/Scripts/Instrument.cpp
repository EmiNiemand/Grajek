//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/Instrument.h"
#include <utility>
#include "Components/UI/Image.h"

void Instrument::Setup(InstrumentName instrumentName, MusicGenre musicGenre, const std::shared_ptr<Image> &image) {
    this->name = instrumentName;
    this->genre = musicGenre;
    this->icon = image;
    patterns = std::vector<std::shared_ptr<MusicPattern>>();
}

void Instrument::AddPatterns(std::vector<std::shared_ptr<MusicPattern>> newPatterns) { patterns = std::move(newPatterns); }

void Instrument::GeneratePattern(std::vector<RawSample> newPattern) {
    auto pattern = std::make_shared<MusicPattern>();
    pattern->instrumentName = name;
    pattern->id = (int)patterns.size();
    // Make sure that first sample has zero delay, otherwise pattern recognition won't work
    newPattern.begin()->delay = 0;
    for (auto soundRaw : newPattern) {
        pattern->sounds.push_back(std::make_shared<Sound>(samples[soundRaw.sample], soundRaw.delay, soundRaw.duration));
    }
    patterns.push_back(pattern);
}

void Instrument::GenerateOpponentPattern(std::vector<RawSample> newPattern) {
    auto pattern = std::make_shared<MusicPattern>();
    pattern->instrumentName = name;
    pattern->id = (int)opponentPatterns.size();
    // Make sure that first sample has zero delay, otherwise pattern recognition won't work
    newPattern.begin()->delay = 0;
    for (auto soundRaw : newPattern) {
        pattern->sounds.push_back(std::make_shared<Sound>(monoSamples[soundRaw.sample], soundRaw.delay, soundRaw.duration));
    }
    opponentPatterns.push_back(pattern);
}

void Instrument::AddSamples(const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
        samples.push_back(std::make_shared<Sample>(samples.size(), path));
    }
}

void Instrument::AddMonoSamples(const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
        monoSamples.push_back(std::make_shared<Sample>(monoSamples.size(), path));
    }
}

std::string Instrument::NameToString() const {
    switch (name) {
        case Clap: return "Clap";
        case Drums: return "Drums";
        case Trumpet: return "Trumpet";
        case Launchpad: return "Launchpad";
        case Guitar: return "Guitar";
        default: return "";
    }
}

std::shared_ptr<Instrument> Instrument::GetInstrument(InstrumentName instrumentName) {
    auto instrument = std::make_shared<Instrument>();

    // Add samples
    // -----------
    switch (instrumentName) {
        case Clap:
            instrument->Setup(instrumentName, MusicGenre::Rhythmic);
            instrument->AddSamples({
               "res/sounds/direct/clap/clapWeak.wav",
               "res/sounds/direct/clap/clapStrong.wav"});
            instrument->AddMonoSamples({
                "res/sounds/positional/clap/clapWeak_mono.wav",
                "res/sounds/positional/clap/clapStrong_mono.wav"});
            break;
        case Drums:
            instrument->Setup(instrumentName, MusicGenre::Jazz);
            instrument->AddSamples({
               "res/sounds/direct/drums/hat.wav",
               "res/sounds/direct/drums/kick.wav",
               "res/sounds/direct/drums/snare.wav"});
            instrument->AddMonoSamples({
                "res/sounds/positional/drums/hat_mono.wav",
                "res/sounds/positional/drums/kick_mono.wav",
                "res/sounds/positional/drums/snare_mono.wav"});
            break;
            // TODO: actually implement these three
        case Trumpet:
            instrument->Setup(instrumentName, MusicGenre::RnB);
            instrument->AddSamples({
               "res/sounds/direct/trumpet/lowPitched.wav",
               "res/sounds/direct/trumpet/mediumPitched.wav",
               "res/sounds/direct/trumpet/highPitched.wav"});
            instrument->AddMonoSamples({
                "res/sounds/positional/trumpet/lowPitched_mono.wav",
                "res/sounds/positional/trumpet/mediumPitched_mono.wav",
                "res/sounds/positional/trumpet/highPitched_mono.wav"});
            break;
        case Launchpad:
            instrument->Setup(instrumentName, MusicGenre::SynthPop);
            break;
        case Guitar:
            instrument->Setup(instrumentName, MusicGenre::Rock);
            break;
    }

    // Add patterns
    // ------------
    switch (instrumentName) {
        case Clap:
            // 1      *
            // 0  * *
            instrument->GeneratePattern({
                {0, 0}, {0, 0.5}, {1, 0.5}});
            // 1      *
            // 0  *
            instrument->GeneratePattern({
                {0, 0}, {1, 1}});
            // 1  *  **
            // 0
            instrument->GeneratePattern({
                {1, 0}, {1, 1}, {1, 0.5}});
            //
            // Opponent patterns
            //
            // 1      *
            // 0  * *
            instrument->GenerateOpponentPattern({
                                                {0, 0}, {0, 0.5}, {1, 0.5}});
            // 1      *
            // 0  *
            instrument->GenerateOpponentPattern({
                                                {0, 0}, {1, 1}});
            break;
        case Drums:
            // 2      *
            // 1  *
            // 0
            instrument->GeneratePattern({
                {1, 0}, {2, 1}});
            // 2      *
            // 1  * *
            // 0
            instrument->GeneratePattern({
                {1, 0}, {1, 0.5}, {2, 0.5}});
            // 2        *
            // 1  *   *
            // 0    *     *
            instrument->GeneratePattern({
                {1, 0}, {0, 0.5}, {1, 0.5},
                {2, 0.5}, {0, 0.5}});
            // 2      *
            // 1    *   *
            // 0  *
            instrument->GeneratePattern({
                {0, 0}, {1, 0.5}, {2, 0.5},
                {1, 0.5}});
            // 2      *
            // 1  *
            // 0    *   *
            instrument->GeneratePattern({
                {1, 0}, {0, 0.5}, {2, 0.5},
                {0, 0.5}});
            //
            // Opponent patterns
            //
            // 2      *
            // 1  * *
            // 0
            instrument->GenerateOpponentPattern({
                                                {1, 0}, {1, 0.5}, {2, 0.5}});
            // 2        *
            // 1  *   *
            // 0    *     *
            instrument->GenerateOpponentPattern({
                                                {1, 0}, {0, 0.5}, {1, 0.5},
                                                {2, 0.5}, {0, 0.5}});
            // 2      *
            // 1    *   *
            // 0  *
            instrument->GenerateOpponentPattern({
                                                {0, 0}, {1, 0.5}, {2, 0.5},
                                                {1, 0.5}});
            break;
            // TODO: actually implement these three
        case Trumpet:
            // P1
            // 2      ----
            // 1    --
            // 0 -/-
            instrument->GeneratePattern({
                {0, 0, 0.5}, {0, 0, 0.5},
                {1, 0, 1}, {2, 0, 2}});
            // P2
            // 2 -
            // 1  ---
            // 0     --/--
            instrument->GeneratePattern({
                {2, 0, 0.5}, {1, 0, 1.5},
                {0, 0, 1}, {0, 0, 1}});
            // P3
            // 2     -- -
            // 1       -
            // 0 ----
            instrument->GeneratePattern({
                {0, 0, 2}, {2, 0, 1},
                {1, 0, 0.5}, {2, 0, 0.5}});
            // P4
            // 2       -
            // 1     -- -
            // 0 ----
            instrument->GeneratePattern({
                {0, 0, 2}, {1, 0, 1},
                {2, 0, 0.5}, {1, 0, 0.5}});

            // SECRETS
            // Smoke On The Water
            // 2     ----    -
            // 1   --      --
            // 0 --      --
            instrument->GeneratePattern({
                {0, 0, 1}, {1, 0, 1},
                {2, 0, 2},
                {0, 0, 1}, {1, 0, 1},
                {2, 0, 0.5}});
            // 2        ----
            // 1 ---  --    --
            // 0    --        --
            instrument->GeneratePattern({
                {1, 0, 1.5},
                {0, 0, 1}, {1, 0, 1},
                {2, 0, 2}, {1, 0, 1},
                {0, 0, 2}});
            //
            // Opponent patterns
            //
            // P1
            // 2      ----
            // 1    --
            // 0 -/-
            instrument->GenerateOpponentPattern({
                                                {0, 0, 0.5}, {0, 0, 0.5},
                                                {1, 0, 1}, {2, 0, 2}});
            // P2
            // 2 -
            // 1  ---
            // 0     --/--
            instrument->GenerateOpponentPattern({
                                                {2, 0, 0.5}, {1, 0, 1.5},
                                                {0, 0, 1}, {0, 0, 1}});
            // P3
            // 2     -- -
            // 1       -
            // 0 ----
            instrument->GenerateOpponentPattern({
                                                {0, 0, 2}, {2, 0, 1},
                                                {1, 0, 0.5}, {2, 0, 0.5}});
            break;
        case Launchpad:
            break;
        case Guitar:
            break;
    }

    return instrument;
}