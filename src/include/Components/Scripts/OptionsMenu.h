#ifndef GLOOMENGINE_OPTIONSMENU_H
#define GLOOMENGINE_OPTIONSMENU_H

#include "Components/Component.h"
#include "glm/vec2.hpp"

class Button;

class OptionsMenu : public Component {
private:
    std::shared_ptr<Button> activeButton;

public:
    OptionsMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~OptionsMenu() override;

    void Start() override;
    void ShowMenu();
    void HideMenu();
    void ChangeActiveButton(glm::vec2 moveVector);
    void OnClick();
};

#endif //GLOOMENGINE_OPTIONSMENU_H
