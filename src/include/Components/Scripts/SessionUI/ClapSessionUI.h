#ifndef GLOOMENGINE_CLAPSESSIONUI_H
#define GLOOMENGINE_CLAPSESSIONUI_H

#include "SessionUI.h"

class ClapSessionUI : public SessionUI {
public:
    ClapSessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void PlaySound(int index) override;
};

#endif //GLOOMENGINE_CLAPSESSIONUI_H