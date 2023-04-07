//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/Instrument.h"

#include <utility>
#include "Components/UI/Image.h"
#include "Components/Scripts/MusicPattern.h"

void Instrument::Setup(InstrumentName name, MusicGenre genre, const std::shared_ptr<Image> &icon) {
    this->name = name;
    this->genre = genre;
    this->icon = icon;
}

void Instrument::AddPatterns(std::vector<std::shared_ptr<MusicPattern>> newPatterns) { patterns = std::move(newPatterns); }

