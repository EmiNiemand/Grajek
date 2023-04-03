//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_INSTRUMENT_H
#define GLOOMENGINE_INSTRUMENT_H

#include <memory>
#include <vector>

class Image;

enum InstrumentName { Clap, Drums };
enum MusicGenre { Jazz = 80, RnB = 110 };

class Instrument {
private:

public:
    InstrumentName name;
    MusicGenre genre;
    std::shared_ptr<Image> icon;

    Instrument(InstrumentName name, MusicGenre genre=MusicGenre::Jazz, const std::shared_ptr<Image> &icon = nullptr);
    void AddPatterns(std::vector<MusicPattern>());
};


#endif //GLOOMENGINE_INSTRUMENT_H
