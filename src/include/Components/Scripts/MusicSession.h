//
// Created by masterktos on 06.04.23.
//

#ifndef GLOOMENGINE_MUSICSESSION_H
#define GLOOMENGINE_MUSICSESSION_H


#include <memory>
#include <utility>
#include <vector>
#include "Components/Component.h"

class MusicPattern;
class PlayerManager;
class SessionUI;
class Sound;
class Instrument;

class MusicSession : public Component {
private:
    int bpm;
    std::shared_ptr<SessionUI> sessionUI;
    std::shared_ptr<PlayerManager> playerManager;

    std::vector<Sound> recordedSounds;
    std::vector<std::shared_ptr<MusicPattern>> potentialPatterns;
    float lastTime = 0;

    float timeoutCounter = 0.0f;
    float timeout = 3.0f;

public:
    std::shared_ptr<Instrument> instrument;

    MusicSession(const std::shared_ptr<GameObject> &parent, int id);
    void Setup(std::shared_ptr<Instrument> playerInstrument);
    void Update() override;

    void PlaySample(int index);
    void ToggleCheatSheet();

private:
    void DetectPattern();
    void CalcAccuracyAndReset(const std::shared_ptr<MusicPattern>& goodPattern);

    void PatternSuccess(std::shared_ptr<MusicPattern> pattern, float accuracy);
    void PatternTimeout();
    void PatternFail();

    float GetRhythmValue(float currentNoteLength);
};


#endif //GLOOMENGINE_MUSICSESSION_H
