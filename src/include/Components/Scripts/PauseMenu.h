#ifndef GLOOMENGINE_PAUSEMENU_H
#define GLOOMENGINE_PAUSEMENU_H

#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"

class Button;

class PauseMenu : public Menu {
public:
    bool gameShouldExit = false;

    PauseMenu(const std::shared_ptr<GameObject> &parent, uint32_t id);
    ~PauseMenu() override;

    void ShowMenu();
    void OnClick() override;
};

#endif //GLOOMENGINE_PAUSEMENU_H