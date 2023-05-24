#ifndef IMAGE_CPP_OPPONENT_H
#define IMAGE_CPP_OPPONENT_H

#include "Components/Component.h"

class Instrument;
class MusicPattern;
class PlayerManager;
class OpponentManager;
class AudioSource;
struct RawSample;

class Opponent : public Component {
private:
    std::shared_ptr<Instrument> instrument;
    std::shared_ptr<MusicPattern> pattern;
    std::vector<std::shared_ptr<AudioSource>> sampleSources;
    short sampleIndex = 0;
    float accuracy = 0.0f;
    float timer = 0.0f;
    bool triggerActive = false;
    bool defeated = false;

public:
    float satisfactionAverage = 0.0f;
    short wellPlayedPatternCount = 0;

public:
    Opponent(const std::shared_ptr<GameObject> &parent, int id);
    ~Opponent() override;

    void OnCreate() override;
    void OnDestroy() override;
    void Setup(std::shared_ptr<Instrument> instrument1, std::vector<RawSample> musicPattern, float accuracy1);
    void Update() override;
    void OnTriggerEnter(const std::shared_ptr<GameObject>& gameObject) override;
    void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject) override;
    void UpdateSatisfaction(float satisfaction);
};

#endif //IMAGE_CPP_OPPONENT_H