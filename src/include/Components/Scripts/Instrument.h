//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_INSTRUMENT_H
#define GLOOMENGINE_INSTRUMENT_H

enum InstrumentName { Clap, Drums };
enum MusicGenre { Jazz = 80, RnB = 110 };

class Instrument {
private:

public:
    InstrumentName name;
    MusicGenre genre;

    void CreateInstrument(InstrumentName instrumentName, MusicGenre musicGenre);
//    void AddPattern(std::vector<Pattern>());
};


#endif //GLOOMENGINE_INSTRUMENT_H
