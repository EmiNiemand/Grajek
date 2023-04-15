//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_MUSICPATTERN_H
#define GLOOMENGINE_MUSICPATTERN_H

#include <utility>
#include <vector>
#include <memory>
#include "Components/Audio/AudioSource.h"

enum InstrumentName { Clap, Drums };
enum MusicGenre { Jazz = 80, RnB = 110 };

struct Sample
{
	int id;
	//TODO: plug it in when audio is implemented
	std::string clipPath;

    Sample(int id, std::string clip) : id(id), clipPath(std::move(clip)) {}
};

struct Sound
{
	std::shared_ptr<Sample> sample;
	// Delay from the previously played sound; 1 = 1 metronome tact
	float delay;

    Sound(const std::shared_ptr<Sample> &sample, float delay) : sample(sample), delay(delay) {}
};


class MusicPattern {
public:
    InstrumentName instrumentName;
    std::vector<std::shared_ptr<Sound>> sounds;
};


#endif //GLOOMENGINE_MUSICPATTERN_H
