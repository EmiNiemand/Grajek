#include "Components/Scripts/Menus/Dialogue.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Renderers/Animator.h"
#include "EngineManagers/DialogueManager.h"

Dialogue::Dialogue(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Dialogue::~Dialogue() = default;

void Dialogue::Awake() {
    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();

    parent->AddComponent<BoxCollider>()->SetOffset({0, 0, 0});
    parent->GetComponent<BoxCollider>()->SetSize({5, 5, 5});
    parent->GetComponent<BoxCollider>()->isTrigger = true;

    image = GameObject::Instantiate("ButtonImage", parent)->AddComponent<Image>();
    image->LoadTexture(1600, 50, "UI/interactionTalk.png");

    dialogue = GameObject::Instantiate("Dialogue", parent->children.begin()->second);
    text1 = GameObject::Instantiate("DialogueText1", dialogue)->AddComponent<Text>();
    text2 = GameObject::Instantiate("DialogueText2", dialogue)->AddComponent<Text>();
    text3 = GameObject::Instantiate("DialogueText3", dialogue)->AddComponent<Text>();
    text1->LoadFont(texts[0].text1, 250, 200, 32);
    text2->LoadFont(texts[0].text2, 250, 150, 32);
    text3->LoadFont(texts[0].text3, 250, 100, 32);
    GameObject::Instantiate("DialogueImage", dialogue)->AddComponent<Image>()->LoadTexture(0, 0, "UI/dialogue.png");
    image->enabled = false;
    dialogue->DisableSelfAndChildren();

    Component::Awake();
}

void Dialogue::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
    if (!forced)
        image->enabled = true;
    Component::OnTriggerStay(gameObject);
}

void Dialogue::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = false;
    image->enabled = false;
    Component::OnTriggerExit(gameObject);
}

void Dialogue::Update() {
    if (!triggerActive || menuActive) return;
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_E) && !forced) {
        if (dialogueIndex == 0)
            ShowDialogue();
    }
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER)) {
        if (!active) return;
        if (dialogueIndex == texts.size() - 1) {
            HideDialogue();
            return;
        }
        NextDialogue();
    }
    Component::Update();
}

void Dialogue::ShowDialogue() {
    text1->text = texts[0].text1;
    text2->text = texts[0].text2;
    text3->text = texts[0].text3;
    active = true;
    playerManager->inputEnabled = false;
    dialogue->EnableSelfAndChildren();
    image->enabled = false;
}

void Dialogue::HideDialogue() {
    active = false;
    dialogueIndex = 0;
    playerManager->inputEnabled = true;
    dialogue->DisableSelfAndChildren();
    if (!forced)
        image->enabled = true;
}

void Dialogue::NextDialogue() {
    dialogueIndex++;
    text1->text = texts[dialogueIndex].text1;
    text2->text = texts[dialogueIndex].text2;
    text3->text = texts[dialogueIndex].text3;
}

void Dialogue::OnCreate() {
    DialogueManager::GetInstance()->dialogues.insert({id, std::dynamic_pointer_cast<Dialogue>(shared_from_this())});
    Component::OnCreate();
}

void Dialogue::OnDestroy() {
    DialogueManager::GetInstance()->dialogues.erase(id);
    text1.reset();
    text2.reset();
    text3.reset();
    dialogue.reset();
    playerManager.reset();
    texts.clear();
    image.reset();
    Component::OnDestroy();
}
