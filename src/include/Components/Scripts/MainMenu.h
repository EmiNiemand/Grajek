#ifndef GLOOMENGINE_MAINMENU_H
#define GLOOMENGINE_MAINMENU_H

#include "Components/UI/Menu.h"

class MainMenu : public Menu {
public:
    bool gameShouldExit = false;

public:
    MainMenu(const std::shared_ptr<GameObject> &parent, uint32_t id);
    ~MainMenu() override;

    void Start() override;
    void ShowMenu();
    void OnClick() override;
};

#endif //GLOOMENGINE_MAINMENU_H