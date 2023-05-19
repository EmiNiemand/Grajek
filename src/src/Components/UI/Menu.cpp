#include "Components/UI/Menu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"

Menu::Menu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Menu::~Menu() = default;

void Menu::HideMenu() {
    parent->DisableSelfAndChildren();
    if (activeButton)
        activeButton->isActive = false;
}

void Menu::ChangeActiveButton(glm::vec2 moveVector) {
    if (moveVector.y == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->previousButton;
        activeButton->isActive = true;
    }
    if (moveVector.y == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->nextButton;
        activeButton->isActive = true;
    }
}

std::shared_ptr<Image> Menu::AddImage(std::string name, int x, int y, const std::string &path) {
    auto image = GameObject::Instantiate(name, parent)->AddComponent<Image>();
	image->LoadTexture(x, y, path);
    return image;
}

std::shared_ptr<Text> Menu::AddText(std::string name, std::string text, int x, int y, FT_UInt fontSize, glm::vec3 color, const std::string &path) {
    auto _text = GameObject::Instantiate(name, parent)->AddComponent<Text>();
    _text->LoadFont(text, x, y, fontSize, color, path);
    return _text;
}

std::shared_ptr<Button> Menu::AddButton(std::string name, int x, int y, const std::string &pathInactive, const std::string &pathActive,
                     std::string text, FT_UInt fontSize, glm::vec3 color, const std::string &fontPath) {
    auto button = GameObject::Instantiate(std::move(name), parent)->AddComponent<Button>();
    button->LoadTexture(x, y, pathInactive, pathActive);
    button->LoadFont(std::move(text), fontSize, color, fontPath);
    return button;
}