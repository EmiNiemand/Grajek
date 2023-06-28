#include "Components/Scripts/Menus/MapTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "EngineManagers/HIDManager.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/DialogueManager.h"

MapTrigger::MapTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

MapTrigger::~MapTrigger() = default;

void MapTrigger::Start() {
    Component::Start();
    auto interactButton = GameObject::Instantiate("InteractButton", parent);
    interactButton->transform->SetLocalPosition({0, 5, 0});
    buttonImage = GameObject::Instantiate("ButtonImage", interactButton)->AddComponent<Image>();
    buttonImage->LoadTexture(0, 0, "UI/openMap.png");
    buttonImage->isDynamic = true;
    buttonImage->SetScale(0.5);
    buttonImage->enabled = false;

    background = GameObject::Instantiate("MapBackground", parent)->AddComponent<Image>();
    background->LoadTexture(0, 0, "UI/backgroundOpacity60.png", 0.95);
    background->enabled = false;
    mapImage = GameObject::Instantiate("MapImage", parent)->AddComponent<Image>();
    mapImage->LoadTexture(300, 97, "UI/Mapa_Popup.png", 0.9);
    mapImage->enabled = false;

    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    DialogueManager::GetInstance()->map = std::dynamic_pointer_cast<MapTrigger>(shared_from_this());
}

void MapTrigger::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
    buttonImage->enabled = true;
    Component::OnTriggerEnter(gameObject);
}

void MapTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = false;
    buttonImage->enabled = false;
    Component::OnTriggerExit(gameObject);
}

void MapTrigger::Update() {
    if (!triggerActive && !background->enabled) return;
    auto hid = HIDManager::GetInstance();

    if (hid->IsKeyDown(Key::KEY_E)) {
        playerManager->inputEnabled = false;
        buttonImage->enabled = false;
        background->enabled = true;
        mapImage->enabled = true;
    }

    if (hid->IsKeyDown(Key::KEY_ESC)) {
        playerManager->inputEnabled = true;
        if (triggerActive)
            buttonImage->enabled = true;
        background->enabled = false;
        mapImage->enabled = false;
    }
    Component::Update();
}

void MapTrigger::OnDestroy() {
    buttonImage.reset();
    background.reset();
    mapImage.reset();
    playerManager.reset();
    Component::OnDestroy();
}