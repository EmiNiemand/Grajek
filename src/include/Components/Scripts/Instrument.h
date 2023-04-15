//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_INSTRUMENT_H
#define GLOOMENGINE_INSTRUMENT_H

#include <memory>
#include <vector>
#include "MusicPattern.h"

class Image;

class Instrument {
private:

public:
    InstrumentName name;
    MusicGenre genre;
    std::shared_ptr<Image> icon;

	std::vector<std::shared_ptr<MusicPattern>> patterns;
	std::vector<std::shared_ptr<Sample>> samples;

    void Setup(InstrumentName name, MusicGenre genre=MusicGenre::Jazz, const std::shared_ptr<Image> &icon = nullptr);
    void AddPatterns(std::vector<std::shared_ptr<MusicPattern>> newPatterns);
};


#endif //GLOOMENGINE_INSTRUMENT_H
