//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_MUSICPATTERN_H
#define GLOOMENGINE_MUSICPATTERN_H

#include <vector>
#include <memory>

enum InstrumentName { Clap, Drums };
enum MusicGenre { Jazz = 80, RnB = 110 };

// AudioClip but fancy
struct Sample
{
	int id;
	//TODO: plug it in when audio is implemented
	//AudioClip clip;
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
