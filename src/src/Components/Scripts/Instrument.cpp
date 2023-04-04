//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/MusicPattern.h"

Instrument::Instrument(InstrumentName name, MusicGenre genre, const std::shared_ptr<Image> &icon)
        : name(name), genre(genre), icon(icon) {

}

