#ifndef GLOOMENGINE_TRUMPETSESSIONUI_H
#define GLOOMENGINE_TRUMPETSESSIONUI_H

#include "SessionUI.h"

class TrumpetSessionUI : public SessionUI {
private:
    bool soundsToFade[3];
    glm::vec2 valveInitPos[3];
public:
    TrumpetSessionUI(const std::shared_ptr<GameObject> &parent, int id);
    void Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples,
               bool sessionMetronomeSound, bool sessionMetronomeVisuals, bool sessionBackingTrack) override;

    void Update() override;

    void PlaySound(int index) override;
    void StopSound(int index) override;

};

#endif //GLOOMENGINE_TRUMPETSESSIONUI_H