#include "Components/UI/Menu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "Components/Audio/AudioSource.h"
#include <ft2build.h>
#include FT_FREETYPE_H

Menu::Menu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Menu::~Menu() = default;

void Menu::Awake() {
    auto soundObject = GameObject::Instantiate("buttonChangeAudioSource", parent);
    activeButtonChangeSound = soundObject->AddComponent<AudioSource>();
    activeButtonChangeSound->LoadAudioData("res/sounds/direct/options_scroll.wav", AudioType::Direct);
    activeButtonChangeSound->SetGain(0.5);
    Component::Awake();
}

void Menu::OnDestroy() {
    activeButton.reset();
    activeButtonChangeSound.reset();
    Component::OnDestroy();
}

bool Menu::ShowMenu() {
    if(GetParent()->GetEnabled()) return false;

    parent->EnableSelfAndChildren();
    GloomEngine::GetInstance()->timeScale = 0;
    return true;
}

void Menu::HideMenu() {
    if(!GetParent()->GetEnabled()) return;

    parent->DisableSelfAndChildren();
    if (activeButton)
        activeButton->isActive = false;
    GloomEngine::GetInstance()->timeScale = 1;
}

void Menu::ChangeActiveButton(glm::vec2 moveVector) {
    if (moveVector.x == 1.0f) {
        if (!activeButton->right) return;
        activeButton->isActive = false;
        activeButton = activeButton->right;
        activeButton->isActive = true;
        activeButtonChangeSound->ForcePlaySound();
    }
    if (moveVector.x == -1.0f) {
        if (!activeButton->left) return;
        activeButton->isActive = false;
        activeButton = activeButton->left;
        activeButton->isActive = true;
        activeButtonChangeSound->ForcePlaySound();
    }
    if (moveVector.y == 1.0f) {
        if (!activeButton->up) return;
        activeButton->isActive = false;
        activeButton = activeButton->up;
        activeButton->isActive = true;
        activeButtonChangeSound->ForcePlaySound();
    }
    if (moveVector.y == -1.0f) {
        if (!activeButton->down) return;
        activeButton->isActive = false;
        activeButton = activeButton->down;
        activeButton->isActive = true;
        activeButtonChangeSound->ForcePlaySound();
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


