#ifndef GLOOMENGINE_PAUSEMENU_H
#define GLOOMENGINE_PAUSEMENU_H

#include "Components/Component.h"
#include "glm/vec2.hpp"

class Button;

class PauseMenu : public Component {
private:
    std::shared_ptr<Button> activeButton;

public:
    PauseMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~PauseMenu() override;

    void Start() override;
    void ShowMenu();
    void HideMenu();
    void ChangeActiveButton(glm::vec2 moveVector);
    void OnClick();
};

#endif //GLOOMENGINE_PAUSEMENU_H