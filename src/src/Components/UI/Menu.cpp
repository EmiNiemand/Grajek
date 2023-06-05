#include "Components/UI/Menu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include <ft2build.h>
#include FT_FREETYPE_H

Menu::Menu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Menu::~Menu() = default;

bool Menu::ShowMenu() {
    if(GetParent()->GetEnabled()) return false;

    parent->EnableSelfAndChildren();
    GloomEngine::GetInstance()->timeScale = 0;
    return true;
}

void Menu::HideMenu() {
    parent->DisableSelfAndChildren();
    if (activeButton)
        activeButton->isActive = false;
    GloomEngine::GetInstance()->timeScale = 1;
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

std::shared_ptr<Image> Menu::AddImage(std::string name, int x, int y, const std::string &path, float z) {
    auto image = GameObject::Instantiate(name, parent)->AddComponent<Image>();
	image->LoadTexture(x, y, path, z);
    return image;
}

std::shared_ptr<Text> Menu::AddText(std::string name, std::string text, int x, int y, int fontSize, glm::vec3 color, const std::string &path) {
    auto _text = GameObject::Instantiate(name, parent)->AddComponent<Text>();
    _text->LoadFont(text, x, y, fontSize, color, path);
    return _text;
}

std::shared_ptr<Button> Menu::AddButton(std::string name, int x, int y, const std::string &pathInactive, const std::string &pathActive,
                     std::string text, int fontSize, glm::vec3 color, const std::string &fontPath, float z) {
    auto button = GameObject::Instantiate(std::move(name), parent)->AddComponent<Button>();
    button->LoadTexture(x, y, pathInactive, pathActive, z);
    button->LoadFont(std::move(text), fontSize, color, fontPath);
    return button;
}

void Menu::OnDestroy() {
    activeButton.reset();
    Component::OnDestroy();
}
