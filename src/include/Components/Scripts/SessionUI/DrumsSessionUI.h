#ifndef GLOOMENGINE_DRUMSSESSIONUI_H
#define GLOOMENGINE_DRUMSSESSIONUI_H

#include "SessionUI.h"

class DrumsSessionUI : public SessionUI {
public:
    DrumsSessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void PlaySound(int index) override;

    void Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, std::string metronomePath) override;
};

#endif //GLOOMENGINE_DRUMSSESSIONUI_H