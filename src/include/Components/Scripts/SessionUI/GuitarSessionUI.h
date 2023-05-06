#ifndef GLOOMENGINE_GUITARSESSIONUI_H
#define GLOOMENGINE_GUITARSESSIONUI_H

#include "SessionUI.h"

class GuitarSessionUI : public SessionUI {
public:
    GuitarSessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void PlaySound(int index) override;
};

#endif //GLOOMENGINE_GUITARSESSIONUI_H