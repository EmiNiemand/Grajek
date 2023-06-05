#include "Components/Scripts/Menus/CreditsMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"

CreditsMenu::CreditsMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

CreditsMenu::~CreditsMenu() = default;

bool CreditsMenu::ShowMenu() {
    return Menu::ShowMenu();
}