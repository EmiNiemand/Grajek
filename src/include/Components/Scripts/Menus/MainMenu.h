#ifndef GLOOMENGINE_MAINMENU_H
#define GLOOMENGINE_MAINMENU_H

#include "Components/UI/Menu.h"

class MainMenu : public Menu {
public:
    bool gameShouldExit = false;

public:
    MainMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~MainMenu() override;

    void Start() override;
    bool ShowMenu() override;
    void OnClick() override;
};

#endif //GLOOMENGINE_MAINMENU_H