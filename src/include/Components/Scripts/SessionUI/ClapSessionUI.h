#ifndef GLOOMENGINE_CLAPSESSIONUI_H
#define GLOOMENGINE_CLAPSESSIONUI_H

#include "SessionUI.h"

class ClapSessionUI : public SessionUI {
public:
    ClapSessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void Setup(std::shared_ptr<Instrument> instrument) override;
    void PlaySound(int index) override;
};

#endif //GLOOMENGINE_CLAPSESSIONUI_H