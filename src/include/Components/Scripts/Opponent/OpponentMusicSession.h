#ifndef GAME_CPP_OPPONENTMUSICSESSION_H
#define GAME_CPP_OPPONENTMUSICSESSION_H

#include <memory>
#include <utility>
#include <vector>
#include "Components/Component.h"

class MusicPattern;
class SessionUI;
class Sound;
class Instrument;

class OpponentMusicSession : public Component {
private:
    int bpm = 0;
    std::shared_ptr<SessionUI> sessionUI;

    std::vector<Sound> recordedSounds;
    std::vector<std::shared_ptr<MusicPattern>> potentialPatterns;
    float lastTime = 0;

    float timeoutCounter = 0.0f;
    float timeout = 3.0f;

public:
    std::shared_ptr<Instrument> instrument;

    OpponentMusicSession(const std::shared_ptr<GameObject> &parent, int id);
    void Setup(std::shared_ptr<Instrument> playerInstrument);
    void Update() override;

    void OnDestroy() override;

    void PlaySample(int index);
    void StopSample(int index);
    void ToggleCheatSheet();
    void ToggleInstrumentControl();

    void ToggleMetronomeVisuals();
    void ToggleMetronomeSound();

private:
    void DetectPattern();
    void CalcAccuracyAndReset(const std::shared_ptr<MusicPattern>& goodPattern);

    void PatternSuccess(const std::shared_ptr<MusicPattern>& pattern, float accuracy);
    void PatternTimeout();
    void PatternFail();

    float GetRhythmValue(float currentNoteLength) const;
};

#endif //GAME_CPP_OPPONENTMUSICSESSION_H