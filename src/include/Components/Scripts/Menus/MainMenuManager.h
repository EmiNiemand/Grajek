#ifndef GLOOMENGINE_MAINMENUMANAGER_H
#define GLOOMENGINE_MAINMENUMANAGER_H

#include "Components/Component.h"
#include "glm/vec2.hpp"
#include "MainMenu.h"
#include "LoadGameMenu.h"

class Menu;

class MainMenuManager : public Component {
public:
    std::shared_ptr<Menu> activeMenu;
    std::shared_ptr<MainMenu> mainMenu;
    std::shared_ptr<LoadGameMenu> loadGameMenu;
//    std::shared_ptr<MainMenuOptionsMenu> optionsMenu;
//    std::shared_ptr<CreditsMenu> creditsMenu;

public:
    void Start() override;
    void Update() override;
    void OnDestroy() override;

private:

    void PollInput();

    void OnMenuToggle();
    void OnApply() const;
    void OnUIMove(glm::vec2 moveVector) const;

public:
    MainMenuManager(const std::shared_ptr<GameObject> &parent, int id);
};

#endif //GLOOMENGINE_MAINMENUMANAGER_H