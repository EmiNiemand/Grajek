#include "Components/UI/Menu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"

Menu::Menu(const std::shared_ptr<GameObject> &parent, uint32_t id) : Component(parent, id) {}

Menu::~Menu() = default;

void Menu::HideMenu() {
    parent->DisableSelfAndChildren();
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

std::shared_ptr<GameObject> Menu::AddImage(std::string name, int x, int y, const std::string &path) {
    std::shared_ptr<GameObject> image = GameObject::Instantiate(name, parent);
    image->AddComponent<Image>()->LoadTexture(x, y, path);
    return image;
}

std::shared_ptr<GameObject> Menu::AddText(std::string name, std::string text, int x, int y, FT_UInt fontSize, glm::vec3 color, const std::string &path) {
    std::shared_ptr<GameObject> _text = GameObject::Instantiate(name, parent);
    _text->AddComponent<Text>();
    _text->GetComponent<Text>()->LoadFont(text, x, y, fontSize, color, path);
    return _text;
}

std::shared_ptr<GameObject> Menu::AddButton(std::string name, int x, int y, const std::string &pathInactive, const std::string &pathActive,
                     std::string text, FT_UInt fontSize, glm::vec3 color, const std::string &fontPath) {
    std::shared_ptr<GameObject> button = GameObject::Instantiate(name, parent);
    button->AddComponent<Button>();
    button->GetComponent<Button>()->LoadTexture(x, y, pathInactive, pathActive);
    button->GetComponent<Button>()->LoadFont(text, fontSize, color, fontPath);
    return button;
}