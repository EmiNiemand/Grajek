#ifndef GLOOMENGINE_MAINMENUMANAGER_H
#define GLOOMENGINE_MAINMENUMANAGER_H

#include "Components/Component.h"
#include "glm/vec2.hpp"
#include "MainMenu.h"

class Menu;
class MainMenuOptionsMenu;

class MainMenuManager : public Component {
private:
    std::shared_ptr<Menu> activeMenu;
    std::shared_ptr<MainMenu> mainMenu;
//    std::shared_ptr<NewGameMenu> newGameMenu;
//    std::shared_ptr<LoadGameMenu> loadGameMenu;
//    std::shared_ptr<MainMenuOptionsMenu> optionsMenu;
//    std::shared_ptr<CreditsMenu> creditsMenu;

private:
    void Start() override;
    void Update() override;

    void PollInput();

    void OnMenuToggle();
    void OnApply();
    void OnUIMove(glm::vec2 moveVector);

public:
    MainMenuManager(const std::shared_ptr<GameObject> &parent, uint32_t id);
};

#endif //GLOOMENGINE_MAINMENUMANAGER_H