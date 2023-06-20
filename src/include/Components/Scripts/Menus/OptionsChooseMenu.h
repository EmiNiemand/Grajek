#ifndef GAME_CPP_OPTIONSCHOOSEMENU_H
#define GAME_CPP_OPTIONSCHOOSEMENU_H

#include "Components/Component.h"

class Button;

class OptionsChooseMenu : public Component {
public:
    std::shared_ptr<Button> button1;
    std::shared_ptr<Button> button2;

public:
    OptionsChooseMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~OptionsChooseMenu() override;

    void Update() override;
    void ShowChooseMenu();
    void OnDestroy() override;
};

#endif //GAME_CPP_OPTIONSCHOOSEMENU_H