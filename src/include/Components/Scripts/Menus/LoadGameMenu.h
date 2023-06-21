#ifndef GLOOMENGINE_LOADGAMEMENU_H
#define GLOOMENGINE_LOADGAMEMENU_H

#include "Components/UI/Menu.h"

class LoadGameMenu : public Menu {
public:
    std::string file;

public:
    LoadGameMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~LoadGameMenu() override;

    bool ShowMenu() override;
    void OnClick() override;
};

#endif //GLOOMENGINE_LOADGAMEMENU_H