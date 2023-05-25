#ifndef GLOOMENGINE_LAUNCHPADSESSIONUI_H
#define GLOOMENGINE_LAUNCHPADSESSIONUI_H

#include "SessionUI.h"

class LaunchpadSessionUI : public SessionUI {
public:
    LaunchpadSessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void PlaySound(int index) override;

    void Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, std::string metronomePath) override;
};

#endif //GLOOMENGINE_LAUNCHPADSESSIONUI_H