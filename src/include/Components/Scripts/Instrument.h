//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_INSTRUMENT_H
#define GLOOMENGINE_INSTRUMENT_H

#define DRUMS_PRICE 100
#define TRUMPET_PRICE 250
#define LAUNCHPAD_PRICE INT_MAX
#define GUITAR_PRICE INT_MAX

#include <memory>
#include <vector>
#include "MusicPattern.h"

class Image;

struct RawSample {int sample = 0; float delay = 0.5; float duration=0; };

class Instrument {
private:

public:
    InstrumentName name;
    MusicGenre genre;
    std::shared_ptr<Image> icon;

	std::vector<std::shared_ptr<MusicPattern>> patterns;
    std::vector<std::shared_ptr<MusicPattern>> opponentPatterns;
	std::vector<std::shared_ptr<Sample>> samples;
    std::vector<std::shared_ptr<Sample>> monoSamples;

    void Setup(InstrumentName instrumentName, MusicGenre musicGenre=MusicGenre::Jazz, const std::shared_ptr<Image> &image = nullptr);
    void AddPatterns(std::vector<std::shared_ptr<MusicPattern>> newPatterns);
    /**
     * @brief Generates new pattern from given RawSamples. Each RawSample contains
     * sample index (in instrument), delay value and sound duration.
     */
    void GeneratePattern(std::vector<RawSample> newPattern);
    void GenerateOpponentPattern(std::vector<RawSample> newPattern);
    void AddSamples(const std::vector<std::string>& paths);
    void AddMonoSamples(const std::vector<std::string> &paths);
    std::string NameToString() const;

    static std::shared_ptr<Instrument> GetInstrument(InstrumentName instrumentName);

};


#endif //GLOOMENGINE_INSTRUMENT_H
