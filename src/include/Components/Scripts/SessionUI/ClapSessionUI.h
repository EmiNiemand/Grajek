#ifndef GLOOMENGINE_CLAPSESSIONUI_H
#define GLOOMENGINE_CLAPSESSIONUI_H

#include "SessionUI.h"

class ClapSessionUI : public SessionUI {
public:
    ClapSessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, bool sessionMetronomeSound, bool sessionMetronomeVisuals, bool sessionBackingTrack) override;
    void PlaySound(int index) override;
    bool ToggleCheatSheet() override;
};

#endif //GLOOMENGINE_CLAPSESSIONUI_H