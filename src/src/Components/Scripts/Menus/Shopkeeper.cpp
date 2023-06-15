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
#include "Components/Animations/UIAnimator.h"

Shopkeeper::Shopkeeper(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Shopkeeper::~Shopkeeper() = default;

void Shopkeeper::Start() {




    // TODO delete
    return;





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
    animator->LoadAnimationModel("Crowd/JazzMan001/JazzMan001.dae");
    animator->SetAnimation("CrowdAnimations/Idle3.dae");
    auto shopkeeperDialogue = GameObject::Instantiate("ShopkeeperDialogue", shopkeeperModel);
    texts.push_back({{"Witaj przybyszu. Jestem sklepikarzem w tym miescie. Pomoge tobie."},
                               {"Mozesz sie poruszac WSADem lub strzalkami."},
                               {"Kupuj instrumenty. Mam nadzieje, ze zdazysz kupic wszystkie instrumenty przed poczatkiem konkursu."}});
    texts.push_back({{"Wystepuj przed publicznoscia, aby zarabiac pieniadze."},
                               {"Nacisnij spacje, zeby zaczac grac."},
                               {""}});
    texts.push_back({{"Pierwszym krokiem jest wybor instrumentu, na ktorym chcesz grac."},
                     {"Pamietaj, ze kazdy lubi inny instrument i gatunek muzyki."},
                     {""}});
    texts.push_back({{"Na kazdym instrumencie gra sie inaczej."},
                     {"Klaszcz klawiszami R i U."},
                     {""}});
    texts.push_back({{"Zniszcz krola muzyki."},
                     {"Nacisnij spacje, zeby wyjsc."},
                     {""}});

    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();

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
    playerManager->inputEnabled = false;
    sampleSources.push_back(GameObject::Instantiate("ShopkeeperSample", dialogue)->AddComponent<AudioSource>());
    sampleSources.back()->LoadAudioData("res/sounds/direct/clap/clapWeak.wav", AudioType::Direct);
    sampleSources.push_back(GameObject::Instantiate("ShopkeeperSample", dialogue)->AddComponent<AudioSource>());
    sampleSources.back()->LoadAudioData("res/sounds/direct/clap/clapStrong.wav", AudioType::Direct);
    door = GloomEngine::GetInstance()->FindGameObjectWithName("Door1");
    Component::Start();
}

void Shopkeeper::Update() {





    // TODO delete
    return;





    if (shopkeeperEvent) return;
    if (parent->children.empty()) {
        shopkeeperEvent = true;
        playerManager->inputEnabled = true;
    }
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_SPACE)) {
        if (dialogueIndex != 1 && dialogueIndex != 4) return;
        sessionActive = !sessionActive;
        if (dialogueIndex == 1) {
            background = GameObject::Instantiate("ShopkeeperBackground", image->GetParent()->parent)->AddComponent<Image>();
            background->LoadTexture(0, 0, "UI/backgroundOpacity90.png", 0.5f);
            clapImage = GameObject::Instantiate("ShopkeeperClapImage", background->GetParent())->AddComponent<Image>();
            clapImage->LoadTexture(832, 558, "UI/Icons/small/iconClap.png");
            circle1 = GameObject::Instantiate("ShopkeeperCircle1", background->GetParent())->AddComponent<Image>();
            circle1->LoadTexture(700, 700, "UI/Sesja/circle.png");
            circle1->pivot = {0.5, 0.5};
            circle1->SetPosition(700, 700);
            circle2 = GameObject::Instantiate("ShopkeeperCircle2", background->GetParent())->AddComponent<Image>();
            circle2->LoadTexture(1200, 700, "UI/Sesja/circle.png");
            circle2->pivot = {0.5, 0.5};
            circle2->SetPosition(1200, 700);

            circleScale1 = GameObject::Instantiate("CircleAnimator", background->GetParent())->AddComponent<UIAnimator>();
            circleScale1->Setup(circle1, {
                    {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
                    {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
            }, AnimationBehaviour::Resetable);
            circleColor1 = GameObject::Instantiate("CircleAnimator", background->GetParent())->AddComponent<UIAnimator>();
            circleColor1->Setup(circle1, {
                    {AnimatedProperty::Color, glm::vec3(0.0f, 0.0f, 1.0f), 0.125f},
                    {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
            }, AnimationBehaviour::Resetable);

            circleScale2 = GameObject::Instantiate("CircleAnimator", background->GetParent())->AddComponent<UIAnimator>();
            circleScale2->Setup(circle2, {
                    {AnimatedProperty::Scale, glm::vec3(1.5f), 0.125f},
                    {AnimatedProperty::Scale, glm::vec3(1.0f), 0.125f}
            }, AnimationBehaviour::Resetable);
            circleColor2 = GameObject::Instantiate("CircleAnimator", background->GetParent())->AddComponent<UIAnimator>();
            circleColor2->Setup(circle2, {
                    {AnimatedProperty::Color, glm::vec3(1.0f, 0.0f, 0.0f), 0.125f},
                    {AnimatedProperty::Color, glm::vec3(1.0f), 0.125f}
            }, AnimationBehaviour::Resetable);

            background->GetParent()->DisableSelfAndChildren();
            background->enabled = true;
            clapImage->enabled = true;
        } else {
            GameObject::Destroy(background->GetParent());
        }

        dialogueIndex++;
        // Load shopkeeper animation
        if (dialogueIndex == texts.size()) {
            GloomEngine::GetInstance()->FindGameObjectWithName("ShopkeeperAnimator")->GetComponent<Animator>()->SetAnimation("CrowdAnimations/Walk.dae");
            parent->GetComponent<BoxCollider>()->enabled = false;
            shopkeeperModel->AddComponent<GameObjectAnimator>()->Setup(shopkeeperModel->transform, {
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, 180.0f, 0.0f), 0.8f},
                    {AnimatedProperty::Position, glm::vec3(0.0f, 0.0f, -5.0f), 4.2f}
            }, false);
            dialogue->DisableSelfAndChildren();
            auto animation = GameObject::Instantiate("DoorAnimation", door);
            animation->AddComponent<GameObjectAnimator>()->Setup(door->transform, {
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f},
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, 90.0f, 0.0f), 2.0f},
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, 0.0f, 0.0f), 0.5f},
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, -90.0f, 0.0f), 2.0f}
            }, false);
            return;
        }
        text1->text = texts[dialogueIndex].text1;
        text2->text = texts[dialogueIndex].text2;
        text3->text = texts[dialogueIndex].text3;
    }
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_R)) {
        if (dialogueIndex != 3 && dialogueIndex != 4) return;
        if (dialogueIndex == 3) NextDialogue();
        sampleSources[0]->ForcePlaySound();
        circleScale1->Reset();
        circleColor1->Reset();
    }
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_U)) {
        if (dialogueIndex != 3 && dialogueIndex != 4) return;
        if (dialogueIndex == 3) NextDialogue();
        sampleSources[1]->ForcePlaySound();
        circleScale2->Reset();
        circleColor2->Reset();
    }
    if (dialogueIndex == 1 || dialogueIndex == 3 || dialogueIndex == 4 || dialogueIndex == 5) return;
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER)) {
        if (dialogueIndex == 2) {
            GameObject::Destroy(clapImage->GetParent());
            background->GetParent()->EnableSelfAndChildren();
        }
        NextDialogue();
    }
    Component::Update();
}

void Shopkeeper::OnDestroy() {
    image.reset();
    text1.reset();
    text2.reset();
    text3.reset();
    dialogue.reset();
    shopkeeperModel.reset();
    door.reset();
    playerManager.reset();
    background.reset();
    circle1.reset();
    circle2.reset();
    circleScale1.reset();
    circleScale2.reset();
    circleColor1.reset();
    circleColor2.reset();
    clapImage.reset();
    sampleSources.clear();
    Component::OnDestroy();
}

void Shopkeeper::NextDialogue() {
    dialogueIndex++;
    text1->text = texts[dialogueIndex].text1;
    text2->text = texts[dialogueIndex].text2;
    text3->text = texts[dialogueIndex].text3;
}

void Shopkeeper::LoadData(std::shared_ptr<GameData> data) {
    shopkeeperEvent = data->shopkeeperEvent;
}

void Shopkeeper::SaveData(std::shared_ptr<GameData> &data) {
    data->shopkeeperEvent = shopkeeperEvent;
}
