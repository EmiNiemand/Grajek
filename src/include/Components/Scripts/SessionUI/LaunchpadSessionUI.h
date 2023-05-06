#ifndef GLOOMENGINE_LAUNCHPADSESSIONUI_H
#define GLOOMENGINE_LAUNCHPADSESSIONUI_H

#include "SessionUI.h"

class LaunchpadSessionUI : public SessionUI {
public:
    LaunchpadSessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void PlaySound(int index) override;
};

#endif //GLOOMENGINE_LAUNCHPADSESSIONUI_H