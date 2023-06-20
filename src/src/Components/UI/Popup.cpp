#include "Components/UI/Popup.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/HIDManager.h"
#include "Utilities.h"

Popup::Popup(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

Popup::~Popup() = default;

void Popup::Setup(int x, int y, const std::string &imagePath, const std::string &buttonPathInactive, const std::string &buttonPathActive) {
    Menu::AddButton("PopupButton", x + 250, y - 60, buttonPathInactive, buttonPathActive, "[Enter]", 28, glm::vec3(0.0f), GameFont::KanitMedium, -0.975f);
    Menu::AddImage("PopupImage", x, y, imagePath, -0.95f);
}

void Popup::Update() {
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER) && playerManager) {
        playerManager->inputEnabled = true;
        GameObject::Destroy(parent);
    }
    if (!playerManager) {
        playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
        playerManager->inputEnabled = false;
    }
    Component::Update();
}

void Popup::OnDestroy() {
    playerManager.reset();
    Menu::OnDestroy();
}