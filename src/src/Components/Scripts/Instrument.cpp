//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/Instrument.h"

#include <utility>
#include "Components/UI/Image.h"

void Instrument::Setup(InstrumentName name, MusicGenre genre, const std::shared_ptr<Image> &icon) {
    this->name = name;
    this->genre = genre;
    this->icon = icon;
    patterns = std::vector<std::shared_ptr<MusicPattern>>();
}

void Instrument::AddPatterns(std::vector<std::shared_ptr<MusicPattern>> newPatterns) { patterns = std::move(newPatterns); }

void Instrument::GeneratePattern(std::vector<RawSample> newPattern) {
    auto pattern = std::make_shared<MusicPattern>();
    pattern->instrumentName = name;
    // Make sure that first sample has zero delay, otherwise pattern recognition won't work
    newPattern.begin()->delay = 0;
    for (auto soundRaw : newPattern) {
        pattern->sounds.push_back(std::make_shared<Sound>(samples[soundRaw.sample], soundRaw.delay, soundRaw.duration));
    }
    patterns.push_back(pattern);
}

void Instrument::AddSamples(const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
        samples.push_back(std::make_shared<Sample>(samples.size(), path));
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
            instrument->Setup(instrumentName, MusicGenre::Rhytmic);
            instrument->AddSamples({
               "res/sounds/direct/clap/clapWeak.wav",
               "res/sounds/direct/clap/clapStrong.wav"});
            break;
        case Drums:
            instrument->Setup(instrumentName, MusicGenre::Jazz);
            instrument->AddSamples({
               "res/sounds/direct/drums/hat.wav",
               "res/sounds/direct/drums/kick.wav",
               "res/sounds/direct/drums/snare.wav"});
            break;
            // TODO: actually implement these three
        case Trumpet:
            instrument->Setup(instrumentName, MusicGenre::RnB);
            instrument->AddSamples({
               "res/sounds/direct/trumpet/lowPitched.wav",
               "res/sounds/direct/trumpet/mediumPitched.wav",
               "res/sounds/direct/trumpet/highPitched.wav"});
            break;
        case Launchpad:
            instrument->Setup(instrumentName, MusicGenre::SynthPop);
            instrument->AddSamples({
               "res/sounds/direct/drums/hat.wav",
               "res/sounds/direct/drums/kick.wav",
               "res/sounds/direct/drums/snare.wav"});
            break;
        case Guitar:
            instrument->Setup(instrumentName, MusicGenre::Rock);
            instrument->AddSamples({
               "res/sounds/direct/drums/hat.wav",
               "res/sounds/direct/drums/kick.wav",
               "res/sounds/direct/drums/snare.wav"});
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
            // 1    *
            // 0  *
            instrument->GeneratePattern({
                {0, 0}, {1, 0.5}});
            break;
        case Drums:
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
            break;
            // TODO: actually implement these three
        case Trumpet:
            // 2       -
            // 1     -   --
            // 0  --
            instrument->GeneratePattern({
                {0, 0, 1.0}, {1, 0.5, 0.5},
                {2, 0.5, 0.5}, {1, 0.5, 1.0}});
            break;
        case Launchpad:
            // 2   * *
            // 1 *     *
            // 0 -------
            instrument->GeneratePattern({
                {0, 0, 2.0},
                {1, 0}, {2, 0.5}, {2, 0.5},
                {1, 0.5}});
            break;
        case Guitar:
            // 2   *   *
            // 1 *   *
            // 0 -------
            instrument->GeneratePattern({
                {0, 0, 2.0},
                {1, 0}, {2, 0.5},
                {1, 0.5}, {2, 0.5}});
            break;
    }

    return instrument;
}