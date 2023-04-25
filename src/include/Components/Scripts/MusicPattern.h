//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_MUSICPATTERN_H
#define GLOOMENGINE_MUSICPATTERN_H

#include <utility>
#include <vector>
#include <memory>
#include "Components/Audio/AudioSource.h"

enum InstrumentName { Clap, Drums, Trumpet, Launchpad, Guitar };
enum MusicGenre { Jazz = 80, RnB = 100, SynthPop=120, Rock=140 };

/// It seems like it could be merged with Sound, but not really;
/// Sound is additionally characterized by delay and duration,
/// Sample defines singular noise that can be made with instrument
struct Sample
{
	int id;
	std::string clipPath;

    Sample(int id, std::string clip) : id(id), clipPath(std::move(clip)) {}
};

struct Sound
{
	std::shared_ptr<Sample> sample;
	// Delay from the previously played sound; 1 = 1 metronome tact
	float delay;
    // How long sound must be played; 0 means skipping the check
    float duration;

    Sound(const std::shared_ptr<Sample> &sample, float delay, float duration=0) : sample(sample), delay(delay), duration(duration) {}
};


class MusicPattern {
public:
    InstrumentName instrumentName;
    std::vector<std::shared_ptr<Sound>> sounds;
};


#endif //GLOOMENGINE_MUSICPATTERN_H
