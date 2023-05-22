#include "Components/Scripts/Menus/Dialogue.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Renderers/Animator.h"
#include "Components/Animations/GameObjectAnimator.h"

Dialogue::Dialogue(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Dialogue::~Dialogue() = default;

void Dialogue::Start() {
    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();

    parent->AddComponent<BoxCollider>()->SetOffset({0, 0, 0});
    parent->GetComponent<BoxCollider>()->SetSize({5, 5, 5});
    parent->GetComponent<BoxCollider>()->isTrigger = true;

    image = GameObject::Instantiate("ButtonImage", parent)->AddComponent<Image>();
    image->LoadTexture(1600, 50, "UI/Sklep/Przycisk.png");

    dialogue = GameObject::Instantiate("Dialogue", parent->children.begin()->second);
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

void Dialogue::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
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
            if (parent->GetName() == "SklepikarzDialogue") {
                GloomEngine::GetInstance()->FindGameObjectWithName("AnimatorSklepikarz")->GetComponent<Animator>()->SetAnimation("AnimsNew/Walk.dae");
                auto sklepikarz = GloomEngine::GetInstance()->FindGameObjectWithName("Sklepikarz");
                parent->GetComponent<BoxCollider>()->enabled = false;
                triggerActive = false;
                sklepikarz->AddComponent<GameObjectAnimator>()->Setup(sklepikarz->transform, {
                        {AnimatedProperty::Rotation, glm::vec3(0.0f, 90.0f, 0.0f), 0.8f},
                        {AnimatedProperty::Position, glm::vec3(5.0f, 0.0f, 0.0f), 4.0f}
                }, false);
            }
            active = false;
            dialogueIndex = 0;
            playerManager->inputEnabled = true;
            image->enabled = true;
            HideDialogue();
            return;
        }
        text1->text = texts[dialogueIndex].text1;
        text2->text = texts[dialogueIndex].text2;
        text3->text = texts[dialogueIndex].text3;
    }
    Component::Update();
}

void Dialogue::ShowDialogue() {
    text1->text = texts[0].text1;
    text2->text = texts[0].text2;
    text3->text = texts[0].text3;
    dialogue->EnableSelfAndChildren();
}

void Dialogue::HideDialogue() {
    dialogue->DisableSelfAndChildren();
}