#include "Components/Scripts/Menus/Shopkeeper.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Renderers/Animator.h"
#include "Components/Renderers/Camera.h"
#include "Components/Animations/GameObjectAnimator.h"
#include "LowLevelClasses/GameData.h"
#include "EngineManagers/SceneManager.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/Animations/UIAnimator.h"
#include "Components/Scripts/Player/PlayerInput.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/UI/Popup.h"
#include "EngineManagers/DialogueManager.h"

Shopkeeper::Shopkeeper(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Shopkeeper::~Shopkeeper() = default;

void Shopkeeper::Start() {
    Component::Start();

    DialogueManager::GetInstance()->shopkeeper = std::dynamic_pointer_cast<Shopkeeper>(shared_from_this());

    if (tutorial) return;

    spaceImage = GameObject::Instantiate("SpaceImage", parent)->AddComponent<Image>();
    spaceImage->LoadTexture(0, 0, "UI/Tutorial/SpaceToPlay.png");
    spaceImage->enabled = false;

    musicSessionImage = GameObject::Instantiate("MusicSessionImage", parent)->AddComponent<Image>();
    musicSessionImage->LoadTexture(0, 0, "UI/Tutorial/MusicSession1.png", -0.99);
    musicSessionImage->enabled = false;

    instrumentControlImage = GameObject::Instantiate("InstrumentControlImage", parent)->AddComponent<Image>();
    instrumentControlImage->LoadTexture(0, 0, "UI/Tutorial/Control1.png", -0.99);
    instrumentControlImage->enabled = false;

    patternsImage = GameObject::Instantiate("PatternsImage", parent)->AddComponent<Image>();
    patternsImage->LoadTexture(0, 0, "UI/Tutorial/Patterns1.png", -0.9);
    patternsImage->enabled = false;

    patternsSound = GameObject::Instantiate("PatternSound", parent)->AddComponent<AudioSource>();
    patternsSound->LoadAudioData("res/sounds/direct/clap/pattern2.wav", AudioType::Direct);
    soundButton1 = GameObject::Instantiate("SoundButtonImage", parent)->AddComponent<Image>();
    soundButton1->LoadTexture(1575, 603, "UI/Sesja/clapPatternsInactive.png", -0.99);
    soundButton1->enabled = false;
    soundButton2 = GameObject::Instantiate("SoundButtonImage", parent)->AddComponent<Image>();
    soundButton2->LoadTexture(1575, 458, "UI/Sesja/clapPatternsSelect.png", -0.99);
    soundButton2->enabled = false;
    soundImage = GameObject::Instantiate("SoundImage", parent)->AddComponent<Image>();
    soundImage->LoadTexture(0, 0, "UI/Tutorial/Sound1.png", -0.9);
    soundImage->enabled = false;

    stopMusicSessionImage = GameObject::Instantiate("StopMusicSessionImage", parent)->AddComponent<Image>();
    stopMusicSessionImage->LoadTexture(0, 0, "UI/Tutorial/StopMusicSession1.png", -0.99);
    stopMusicSessionImage->enabled = false;

    crowdImage = GameObject::Instantiate("StopMusicSessionImage", parent)->AddComponent<Image>();
    crowdImage->LoadTexture(0, 0, "UI/Tutorial/Crowd.png", -0.99);
    crowdImage->enabled = false;

    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();

    if (shopkeeperEvent) {
        dialogueIndex = 7;
        spaceImage->enabled = true;
        return;
    }

    shopkeeperModel = GameObject::Instantiate("ShopkeeperModel", parent);
    shopkeeperModel->AddComponent<Rigidbody>()->enabled = false;
    auto collider = shopkeeperModel->GetComponent<BoxCollider>();
    collider->SetOffset({0, 1, 0});
    collider->SetSize({1, 2, 1});
    shopkeeperModel->transform->SetLocalScale({0.5, 0.5, 0.5});
    auto animatorObject = GameObject::Instantiate("ShopkeeperAnimator", shopkeeperModel);
    auto animator = animatorObject->AddComponent<Animator>();
    animator->LoadAnimationModel("Crowd/Shopkeeper/Shopkeeper.dae");
    animator->SetAnimation("CrowdAnimations/Idle3.dae");
    auto shopkeeperDialogue = GameObject::Instantiate("ShopkeeperDialogue", shopkeeperModel);
    texts.push_back({{"Hi! Welcome to Rhythmtown."},
                               {"I'm your local instrument shopkeeper."},
                               {""}});
    texts.push_back({{"Ah, you're here to take part in the competition?"},
                     {"That's great! Simply show me your Badges."},
                     {"...oh, you don't have any?"}});
    texts.push_back({{"If you want to battle with the current Music King,"},
                     {"you need to master your skills by playing on Drums,"},
                     {"Trumpet, Launchpad and Guitar."}});
    texts.push_back({{"That mastery is put to the test when you get into"},
                     {"musical battle with best Instrument's Buskers in the city."},
                     {"They're easy to locate, just look for a big crowd nearby."}});
    texts.push_back({{"If you manage to outperform them, you get an Instrument's"},
                     {"Badge. You need four of them - one for every mentioned"},
                     {"instrument, to be able to battle with the Music King."}});
    texts.push_back({{"For now, explore the city, earn money with your instruments, pay attention"},
                     {"to listening crowd and most importantly have fun! Come back when you'll"},
                     {"have at least $100 for your first big purchase - Drums. Good luck!"}});

    parent->AddComponent<BoxCollider>()->SetOffset({0, 0, 0});
    parent->GetComponent<BoxCollider>()->SetSize({2.5, 2.5, 2.5});
    parent->GetComponent<BoxCollider>()->isTrigger = true;

    image = GameObject::Instantiate("ButtonImage", shopkeeperDialogue)->AddComponent<Image>();
    image->LoadTexture(1600, 50, "UI/Sklep/Przycisk.png", -0.9);

    dialogue = GameObject::Instantiate("Dialogue", image->GetParent());
    text1 = GameObject::Instantiate("DialogueText1", dialogue)->AddComponent<Text>();
    text2 = GameObject::Instantiate("DialogueText2", dialogue)->AddComponent<Text>();
    text3 = GameObject::Instantiate("DialogueText3", dialogue)->AddComponent<Text>();
    text1->LoadFont(texts[0].text1, 237, 175, 32, glm::vec3(1));
    text1->z = -0.55;
    text2->LoadFont(texts[0].text2, 237, 125, 32, glm::vec3(1));
    text2->z = -0.55;
    text3->LoadFont(texts[0].text3, 237, 75, 32, glm::vec3(1));
    text3->z = -0.55;
    dialogue->AddComponent<Button>()->LoadTexture(1430, 210, "UI/Dialogues/name.png", "UI/Dialogues/name.png", -1);
    dialogue->GetComponent<Button>()->LoadFont("Max Reeve-Erbe", 32, glm::vec3(1));
    GameObject::Instantiate("DialogueImage", dialogue)->AddComponent<Image>()->LoadTexture(127, 0, "UI/Dialogues/Dialog" + std::to_string(RandomnessManager::GetInstance()->GetInt(1, 4)) + ".png", -0.5);
    image->enabled = false;
    playerManager->inputEnabled = false;

    door = GloomEngine::GetInstance()->FindGameObjectWithName("Door1");
    Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(0.5f);
}

void Shopkeeper::Update() {
    if (menuActive) return;
    if (tutorial) return;

    auto hid = HIDManager::GetInstance();

    if (hid->IsKeyDown(Key::KEY_SPACE)) {
        if (dialogueIndex == 7) {
            dialogueIndex++;
            spaceImage->enabled = false;
            return;
        }

        if (dialogueIndex == 8) {
            dialogueIndex--;
            spaceImage->enabled = true;
            return;
        }

        if (dialogueIndex == 14) {
            dialogueIndex++;
            stopMusicSessionImage->enabled = false;
            playerManager->inputEnabled = false;
            crowdImage->enabled = true;
            return;
        }
    }

    if (hid->IsKeyDown(Key::KEY_ENTER)) {
        if (dialogueIndex == 8) {
            dialogueIndex++;
            musicSessionImage->enabled = true;
            playerManager->inputEnabled = false;
            return;
        }

        if (dialogueIndex == 11) {
            dialogueIndex++;
            patternsImage->enabled = false;
            soundImage->enabled = true;
            return;
        }

        if (dialogueIndex == 12) {
            dialogueIndex++;
            patternsSound->PlaySound();
            playerManager->inputEnabled = true;
            return;
        }

        if (dialogueIndex == 15) {
            dialogueIndex++;
            playerManager->inputEnabled = true;
            crowdImage->enabled = false;
            tutorial = true;
            return;
        }
    }

    if (hid->IsKeyDown(Key::KEY_LEFT_SHIFT)) {
        if (dialogueIndex == 9) {
            dialogueIndex++;
            musicSessionImage->enabled = false;
            instrumentControlImage->enabled = true;
            return;
        }
    }

    if (hid->IsKeyDown(Key::KEY_TAB)) {
        if (dialogueIndex == 10) {
            dialogueIndex++;
            instrumentControlImage->enabled = false;
            soundButton1->enabled = true;
            soundButton2->enabled = true;
            patternsImage->enabled = true;
            return;
        }
    }

    if (hid->IsKeyDown(Key::KEY_R)) {
        if (dialogueIndex == 13) {
            patternIsGood = true;
            return;
        }
    }

    if (hid->IsKeyDown(Key::KEY_U)) {
        if (dialogueIndex == 13) {
            if (patternIsGood) {
                dialogueIndex++;
                stopMusicSessionImage->enabled = true;
                soundImage->enabled = false;
                soundButton1->enabled = false;
                soundButton2->enabled = false;
            }
            patternIsGood = false;
            return;
        }
    }



    if (shopkeeperEvent) return;

    if (hid->IsKeyDown(Key::KEY_ENTER)) {
        if (dialogueIndex <= 4) {
            NextDialogue();
            return;
        }

        if (dialogueIndex == 5) {
            dialogueIndex++;
            GloomEngine::GetInstance()->FindGameObjectWithName("ShopkeeperAnimator")->GetComponent<Animator>()->SetAnimation("CrowdAnimations/Walk.dae");
            parent->GetComponent<BoxCollider>()->enabled = false;
            shopkeeperModel->AddComponent<GameObjectAnimator>()->Setup(shopkeeperModel->transform, {
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, 180.0f, 0.0f), 0.8f},
                    {AnimatedProperty::Position, glm::vec3(0.0f, 0.0f, -4.0f), 3.0f}
            }, false);
            dialogue->DisableSelfAndChildren();
            auto animation = GameObject::Instantiate("DoorAnimation", door);
            animation->AddComponent<GameObjectAnimator>()->Setup(door->transform, {
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, 90.0f, 0.0f), 2.0f},
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, 0.0f, 0.0f), 0.5f},
                    {AnimatedProperty::Rotation, glm::vec3(0.0f, -90.0f, 0.0f), 2.0f}
            }, false);
            Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(1.0f);

            GameObject::Instantiate("TutorialPopup", parent)->AddComponent<Popup>()->
                    Setup(0, 0, "UI/Tutorial/Tutorial.png",
                          "UI/ButtonTransparent.png", "UI/ButtonTransparent.png", false);

            return;
        }

        if (dialogueIndex == 6) {
            dialogueIndex++;
            shopkeeperEvent = true;
            playerManager->inputEnabled = true;
            spaceImage->enabled = true;
            return;
        }
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
    texts.clear();
    crowdImage.reset();
    spaceImage.reset();
    patternsImage.reset();
    musicSessionImage.reset();
    stopMusicSessionImage.reset();
    soundImage.reset();
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
    tutorial = data->tutorial;
}

void Shopkeeper::SaveData(std::shared_ptr<GameData> &data) {
    data->shopkeeperEvent = shopkeeperEvent;
    data->tutorial = tutorial;
}