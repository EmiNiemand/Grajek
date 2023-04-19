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
        pattern->sounds.push_back(std::make_shared<Sound>(samples[soundRaw.sample], soundRaw.delay));
    }
    patterns.push_back(pattern);
}

void Instrument::AddSamples(const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
        samples.push_back(std::make_shared<Sample>(samples.size()-1, path));
    }
}

std::string Instrument::NameToString() const {
    switch (name) {
        case Clap: return "Clap";
        case Drums: return "Drums";
        default: return "";
    }
}

