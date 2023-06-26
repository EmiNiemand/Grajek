#include "Components/Scripts/Menus/Dialogue.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Renderers/Animator.h"
#include "EngineManagers/DialogueManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/Renderers/Camera.h"
#include "Utilities.h"
#include "Components/Scripts/Player/PlayerInput.h"

Dialogue::Dialogue(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Dialogue::~Dialogue() = default;

void Dialogue::Awake() {
    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();

    parent->AddComponent<BoxCollider>()->SetOffset({0, 0, 0});
    parent->GetComponent<BoxCollider>()->SetSize({5, 5, 5});
    parent->GetComponent<BoxCollider>()->isTrigger = true;

    auto interactButton = GameObject::Instantiate("InteractButton", parent);
    interactButton->transform->SetLocalPosition({0, 8, 0});
    image = GameObject::Instantiate("ButtonImage", interactButton)->AddComponent<Image>();
    image->LoadTexture(0, 0, "UI/interactionTalk.png");
    image->isDynamic = true;
    image->SetScale(0.5);
    image->enabled = false;

    dialogue = GameObject::Instantiate("Dialogue", parent->children.begin()->second);
    text1 = GameObject::Instantiate("DialogueText1", dialogue)->AddComponent<Text>();
    text2 = GameObject::Instantiate("DialogueText2", dialogue)->AddComponent<Text>();
    text3 = GameObject::Instantiate("DialogueText3", dialogue)->AddComponent<Text>();
    text1->LoadFont(texts[0].text1, 237, 175, 32, glm::vec3(1));
    text2->LoadFont(texts[0].text2, 237, 125, 32, glm::vec3(1));
    text3->LoadFont(texts[0].text3, 237, 75, 32, glm::vec3(1));
    dialogue->AddComponent<Button>()->LoadTexture(1425, 210, "UI/Dialogues/name.png", "UI/Dialogues/name.png", -1);
    dialogue->GetComponent<Button>()->LoadFont(name, 36, glm::vec3(1), GameFont::KanitMedium);
    dialogueImage = GameObject::Instantiate("DialogueImage", dialogue)->AddComponent<Image>();
    dialogueImage->LoadTexture(127, 0, "UI/Dialogues/Dialog" + std::to_string(RandomnessManager::GetInstance()->GetInt(1, 4)) + ".png");
    dialogue->DisableSelfAndChildren();

    Component::Awake();
}

void Dialogue::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
    if (!forced)
        image->enabled = true;
    Component::OnTriggerEnter(gameObject);
}

void Dialogue::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = false;
    image->enabled = false;
    Component::OnTriggerExit(gameObject);
}

void Dialogue::Update() {
    if (menuActive) return;
    if (triggerActive) {
        for (const auto& interactKey : PlayerInput::Interact) {
            if (HIDManager::GetInstance()->IsKeyDown(interactKey.first) && !forced && !active) {
                if (dialogueIndex == 0) {
                    ShowDialogue();
                    return;
                }
            }
        }
    }
    for (const auto& applyKey : PlayerInput::Apply) {
        if (HIDManager::GetInstance()->IsKeyDown(applyKey.first) && active) {
            if (dialogueIndex == texts.size() - 1) {
                HideDialogue();
                return;
            }
            NextDialogue();
        }
    }
    Component::Update();
}

void Dialogue::ShowDialogue() {
    text1->text = "";
    text2->text = "";
    text3->text = "";
    Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(0.5f);
    text1->text = texts[0].text1;
    text2->text = texts[0].text2;
    text3->text = texts[0].text3;
    active = true;
    playerManager->inputEnabled = false;
    dialogue->EnableSelfAndChildren();
    image->enabled = false;
}

void Dialogue::HideDialogue() {
    Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(1.0f);
    active = false;
    dialogueIndex = 0;
    playerManager->inputEnabled = true;
    dialogue->DisableSelfAndChildren();
    if (!forced && triggerActive)
        image->enabled = true;
}

void Dialogue::NextDialogue() {
    dialogueIndex++;
    text1->text = "";
    text2->text = "";
    text3->text = "";
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
