#include "Components/Scripts/Menus/Shopkeeper.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Renderers/Animator.h"
#include "Components/Animations/GameObjectAnimator.h"
#include "LowLevelClasses/GameData.h"
#include "EngineManagers/SceneManager.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"

Shopkeeper::Shopkeeper(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Shopkeeper::~Shopkeeper() = default;

void Shopkeeper::Start() {
    if (shopkeeperEvent) return;

    // create shopkeeper
    shopkeeperModel = GameObject::Instantiate("ShopkeeperModel", parent);
    shopkeeperModel->AddComponent<Rigidbody>()->enabled = false;
    auto collider = shopkeeperModel->GetComponent<BoxCollider>();
    collider->SetOffset({0, 1, 0});
    collider->SetSize({1, 2, 1});
    shopkeeperModel->transform->SetLocalScale({0.5, 0.5, 0.5});
    auto animatorObject = GameObject::Instantiate("ShopkeeperAnimator", shopkeeperModel);
    auto animator = animatorObject->AddComponent<Animator>();
    animator->LoadAnimationModel("JazzMan001/JazzMan001.dae");
    animator->SetAnimation("AnimsNew/Idle3.dae");
    auto shopkeeperDialogue = GameObject::Instantiate("ShopkeeperDialogue", shopkeeperModel);
    texts.push_back({{"Jestem Sklepu."},
                               {"Mozesz sie poruszac WSAD"},
                               {"Kup instrument."}});
    texts.push_back({{"Graj spacja."},
                               {"Strzelaj przyciskami RUP"},
                               {"Rozwalaj wrogow."}});

    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();

    parent->transform->SetLocalPosition({-2.5, 0, 6});
    parent->AddComponent<BoxCollider>()->SetOffset({0, 0, 0});
    parent->GetComponent<BoxCollider>()->SetSize({2.5, 2.5, 2.5});
    parent->GetComponent<BoxCollider>()->isTrigger = true;

    image = GameObject::Instantiate("ButtonImage", shopkeeperDialogue)->AddComponent<Image>();
    image->LoadTexture(1600, 50, "UI/Sklep/Przycisk.png");

    dialogue = GameObject::Instantiate("Dialogue", image->GetParent());
    text1 = GameObject::Instantiate("DialogueText1", dialogue)->AddComponent<Text>();
    text2 = GameObject::Instantiate("DialogueText2", dialogue)->AddComponent<Text>();
    text3 = GameObject::Instantiate("DialogueText3", dialogue)->AddComponent<Text>();
    text1->LoadFont(texts[0].text1, 250, 200, 32);
    text2->LoadFont(texts[0].text2, 250, 150, 32);
    text3->LoadFont(texts[0].text3, 250, 100, 32);
    GameObject::Instantiate("DialogueImage", dialogue)->AddComponent<Image>()->LoadTexture(0, 0, "UI/dialogue.png");
    image->enabled = false;
    HideDialogue();
    Component::Start();
}

void Shopkeeper::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
    image->enabled = true;
    Component::OnTriggerStay(gameObject);
}

void Shopkeeper::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = false;
    image->enabled = false;
    Component::OnTriggerExit(gameObject);
}

void Shopkeeper::Update() {
    if (!triggerActive) return;
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_E)) {
        if (dialogueIndex == 0) {
            active = true;
            playerManager->inputEnabled = false;
            image->enabled = false;
            ShowDialogue();
        }
    }
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER)) {
        if (!active) return;
        dialogueIndex++;
        if (dialogueIndex == texts.size()) {
            GloomEngine::GetInstance()->FindGameObjectWithName("ShopkeeperAnimator")->GetComponent<Animator>()->SetAnimation("AnimsNew/Walk.dae");
            parent->GetComponent<BoxCollider>()->enabled = false;
            triggerActive = false;
            shopkeeperModel->AddComponent<GameObjectAnimator>()->Setup(shopkeeperModel->transform, {
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, 180.0f, 0.0f), 0.8f},
                    {AnimatedProperty::Position, glm::vec3(0.0f, 0.0f, -2.0f), 2.0f}
            }, false);
            image->enabled = false;
            shopkeeperEvent = true;
            active = false;
            playerManager->inputEnabled = true;
            HideDialogue();
            return;
        }
        text1->text = texts[dialogueIndex].text1;
        text2->text = texts[dialogueIndex].text2;
        text3->text = texts[dialogueIndex].text3;
    }
    Component::Update();
}

void Shopkeeper::ShowDialogue() {
    text1->text = texts[0].text1;
    text2->text = texts[0].text2;
    text3->text = texts[0].text3;
    dialogue->EnableSelfAndChildren();
}

void Shopkeeper::HideDialogue() {
    dialogue->DisableSelfAndChildren();
}

void Shopkeeper::LoadData(std::shared_ptr<GameData> data) {
    shopkeeperEvent = data->shopkeeperEvent;
}

void Shopkeeper::SaveData(std::shared_ptr<GameData> &data) {
    data->shopkeeperEvent = shopkeeperEvent;
}
