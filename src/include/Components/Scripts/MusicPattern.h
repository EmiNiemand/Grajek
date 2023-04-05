//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_MUSICPATTERN_H
#define GLOOMENGINE_MUSICPATTERN_H

#include <vector>
#include "Instrument.h"

// AudioClip but fancy
struct Sample
{
	int id;
	//TODO: plug it in when audio is implemented
	//AudioClip clip;
};

struct Sound
{
	Sample sample;
	// Delay from the previously played sound; 1 = 1 metronome tact
	float delay;
};


class MusicPattern {
public:
	InstrumentName InstrumentName;
    std::vector<Sound> sounds;
};


#endif //GLOOMENGINE_MUSICPATTERN_H
