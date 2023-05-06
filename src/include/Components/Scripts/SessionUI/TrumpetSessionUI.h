#ifndef GLOOMENGINE_TRUMPETSESSIONUI_H
#define GLOOMENGINE_TRUMPETSESSIONUI_H

#include "SessionUI.h"

class TrumpetSessionUI : public SessionUI {
public:
    TrumpetSessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void PlaySound(int index) override;
};

#endif //GLOOMENGINE_TRUMPETSESSIONUI_H