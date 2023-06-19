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

Shopkeeper::Shopkeeper(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Shopkeeper::~Shopkeeper() = default;

void Shopkeeper::Start() {
    if (shopkeeperEvent) return;

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
    texts.push_back({{"Hi! Welcome to Rhythmtown. I am your friendly local instrument shopkeeper."},
                               {"Let me show you some basics. Use [W][A][S][D] or arrows to move around,"},
                               {"but please don't go anywhere yet. I still have some things to say."}});
    texts.push_back({{"Ah, you're here to take part in the competition?"},
                               {"If you want to battle with current Music King,"},
                               {"you need to know how to play on drums, trumpet, launchpad and guitar."}});
    texts.push_back({{"Sounds intimidating? Don't worry!"},
                     {"Let's start simple. Hit [Space] to start playing."},
                     {""}});
    texts.push_back({{"From here you can choose instrument on which you want to play."},
                     {"[A][D] to select, [Enter] to accept"},
                     {"For now you can clap to get people's attention. Hit [Enter]!"}});
    texts.push_back({{"Every instrument has different controls and traits."},
                     {"Don't worry though, I'll describe it to you after you buy it."},
                     {"Also, you can see it anytime when you click [Shift]."}});
    texts.push_back({{"Here, you use [R] and [U] to play. Could you click [Shift] once again?"},
                     {""},
                     {""}});
    texts.push_back({{"Each instrument has various patterns that are possible to play."},
                     {"Press [Tab] to see what you've discovered so far."},
                     {""}});
    texts.push_back({{"Ok, let's try that pattern at the top of the notes."},
                     {"Close this page with [Tab]."},
                     {""}});
    texts.push_back({{"Play [R][R][U]!"},
                     {""},
                     {""}});
    texts.push_back({{"Perfect! There's a long way ahead of you, but I believe you can do it."},
                     {"To get instruments you'll need to earn some money."},
                     {"Basically, you need to make people happy with your music."}});
    texts.push_back({{"Make your music interesting!"},
                     {"Keep your patterns varied and try to play to the beat."},
                     {"Please, come back when you'll have at least $100, to buy your first instrument - drums. Good luck!"}});

    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();

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
    dialogue->GetComponent<Button>()->LoadFont("Sklepu", 32, glm::vec3(1));
    GameObject::Instantiate("DialogueImage", dialogue)->AddComponent<Image>()->LoadTexture(127, 0, "UI/Dialogues/Dialog" + std::to_string(RandomnessManager::GetInstance()->GetInt(1, 4)) + ".png", -0.5);
    image->enabled = false;
    playerManager->inputEnabled = false;


    // Clap session set up
    // -------------------
    {
        background = GameObject::Instantiate("ShopkeeperBackground", image->GetParent()->parent)->AddComponent<Image>();
        background->LoadTexture(0, 0, "UI/backgroundOpacity90.png", 0.5f);

        instrumentControl = GameObject::Instantiate("InstrumentControl",
                                                    background->GetParent())->AddComponent<Image>();
        instrumentControl->LoadTexture(451, -1018, "UI/Sesja/clapControl.png", -0.1);
        cheatSheet = GameObject::Instantiate("CheatSheet", background->GetParent())->AddComponent<Image>();
        cheatSheet->LoadTexture(451, -1018, "UI/Sesja/clapPatterns.png", -0.1);

        clapIcon = GameObject::Instantiate("ShopkeeperClapImage",
                                           background->GetParent())->AddComponent<Image>();
        clapIcon->LoadTexture(832, 558, "UI/Icons/small/iconClap.png");

        for (int i = 0; i < 2; ++i) {
            sampleSources.push_back(GameObject::Instantiate("ShopkeeperSample", background->GetParent())
                                            ->AddComponent<AudioSource>());
            sampleImages.push_back(GameObject::Instantiate("SampleImage", background->GetParent())
                                           ->AddComponent<Image>());
            sampleImages.back()->pivot = {0.5, 0.5};
            sampleImages.back()->SetAlpha(0);
        }
        sampleSources[0]->LoadAudioData("res/sounds/direct/clap/clapWeak.wav", AudioType::Direct);
        sampleSources[1]->LoadAudioData("res/sounds/direct/clap/clapStrong.wav", AudioType::Direct);
        sampleImages[0]->LoadTexture(1000, 600, "UI/Sesja/ClapLeft.png");
        sampleImages[1]->LoadTexture(1560, 600, "UI/Sesja/ClapRight.png");
        // Clap
        // ----
        sampleAnimators.push_back({
              GameObject::Instantiate("ScaleLeftAnimator", background->GetParent())->AddComponent<UIAnimator>(),
              GameObject::Instantiate("ScaleRightAnimator", background->GetParent())->AddComponent<UIAnimator>(),
              GameObject::Instantiate("PositionLeftAnimator", background->GetParent())->AddComponent<UIAnimator>(),
              GameObject::Instantiate("PositionRightAnimator", background->GetParent())->AddComponent<UIAnimator>()
        });
        sampleAnimators[0][0]->Setup(sampleImages[0], {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
        }, AnimationBehaviour::Resetable);
        sampleAnimators[0][1]->Setup(sampleImages[1], {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
        }, AnimationBehaviour::Resetable);
        sampleAnimators[0][2]->Setup(sampleImages[0], {
                {AnimatedProperty::Position, glm::vec3(1000 + sampleImages[0]->GetWidth(), 600, 0), 0},
                {AnimatedProperty::Position, glm::vec3(1000, 600, 0),                               0.2f}
        }, AnimationBehaviour::Resetable);
        sampleAnimators[0][3]->Setup(sampleImages[1], {
                {AnimatedProperty::Position, glm::vec3(1560 - sampleImages[0]->GetWidth(), 600, 0), 0},
                {AnimatedProperty::Position, glm::vec3(1560, 600, 0),                               0.2f}
        }, AnimationBehaviour::Resetable);
        // Stomp
        // -----
        sampleAnimators.push_back({
              GameObject::Instantiate("ScaleLeftAnimator", background->GetParent())->AddComponent<UIAnimator>(),
              GameObject::Instantiate("ScaleRightAnimator", background->GetParent())->AddComponent<UIAnimator>(),
              GameObject::Instantiate("PositionLeftAnimator", background->GetParent())->AddComponent<UIAnimator>(),
              GameObject::Instantiate("PositionRightAnimator", background->GetParent())->AddComponent<UIAnimator>()
        });
        sampleAnimators[1][0]->Setup(sampleImages[0], {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
        }, AnimationBehaviour::Resetable);
        sampleAnimators[1][1]->Setup(sampleImages[1], {
                {AnimatedProperty::Scale, glm::vec3(1.5f), 0},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.2f}
        }, AnimationBehaviour::Resetable);
        sampleAnimators[1][2]->Setup(sampleImages[0], {
                {AnimatedProperty::Position, glm::vec3(1000, 300, 0),   0},
                {AnimatedProperty::Position, glm::vec3(1000, 600, 0), 0.2f}
        }, AnimationBehaviour::Resetable);
        sampleAnimators[1][3]->Setup(sampleImages[1], {
                {AnimatedProperty::Position, glm::vec3(1560, 300, 0),   0},
                {AnimatedProperty::Position, glm::vec3(1560, 600, 0), 0.2f}
        }, AnimationBehaviour::Resetable);
    }
    clapIcon->enabled = false;
    background->enabled = false;


    door = GloomEngine::GetInstance()->FindGameObjectWithName("Door1");
    Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(0.5f);
    Component::Start();
}

void Shopkeeper::Update() {
    if (shopkeeperEvent) return;

    auto hid = HIDManager::GetInstance();

    if (hid->IsKeyDown(Key::KEY_ENTER)) {
        if (dialogueIndex == 0  ||  dialogueIndex == 1  ||  dialogueIndex == 9) {
            NextDialogue();
            return;
        }
        if (dialogueIndex == 3) {
            GameObject::Destroy(clapIcon->GetParent());
            background->GetParent()->EnableSelfAndChildren();
            NextDialogue();
        }
        if (dialogueIndex == 10) {
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
            shopkeeperEvent = true;
            playerManager->inputEnabled = true;
            Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(1.0f);
            return;
        }
    }

    if (hid->IsKeyDown(Key::KEY_SPACE)) {
        if (dialogueIndex == 2) {
            sampleImages[0]->SetAlpha(1);
            sampleImages[1]->SetAlpha(1);
            background->GetParent()->DisableSelfAndChildren();
            background->enabled = true;
            clapIcon->enabled = true;
            NextDialogue();
        }
    }

    if (hid->IsKeyDown(Key::KEY_LEFT_SHIFT)) {
        if (dialogueIndex == 4  ||  dialogueIndex == 5  ||  dialogueIndex == 8) {
            if (GloomEngine::GetInstance()->FindGameObjectWithName("InstrumentControlAnimator")) return;
            if (cheatSheetActive) return;
            instrumentControlActive = !instrumentControlActive;
            if (instrumentControlActive) {
                GameObject::Instantiate("InstrumentControlAnimator", background->GetParent())
                        ->AddComponent<UIAnimator>()->Setup(instrumentControl, {
                        {AnimatedProperty::Position, glm::vec3(451.0f, 225.0f, 0.0f), 0.5f}
                });
            } else {
                GameObject::Instantiate("InstrumentControlAnimator", background->GetParent())
                        ->AddComponent<UIAnimator>()->Setup(instrumentControl, {
                        {AnimatedProperty::Position, glm::vec3(451.0f, -1018.0f, 0.0f), 0.5f}
                });
            }
            if (dialogueIndex != 8)
                NextDialogue();
        }
    }

    if (hid->IsKeyDown(Key::KEY_TAB)) {
        if (dialogueIndex == 6  ||  dialogueIndex == 7  ||  dialogueIndex == 8) {
            if (GloomEngine::GetInstance()->FindGameObjectWithName("CheatSheetAnimator")) return;
            if (instrumentControlActive) return;
            cheatSheetActive = !cheatSheetActive;
            if (cheatSheetActive) {
                GameObject::Instantiate("CheatSheetAnimator", parent->parent)
                        ->AddComponent<UIAnimator>()->Setup(cheatSheet, {
                        {AnimatedProperty::Position, glm::vec3(451.0f, 0.0f, 0.0f), 0.5f}
                });
            } else {
                GameObject::Instantiate("CheatSheetAnimator", parent->parent)
                        ->AddComponent<UIAnimator>()->Setup(cheatSheet, {
                        {AnimatedProperty::Position, glm::vec3(451.0f, -1018.0f, 0.0f), 0.5f}
                });
            }
            if (dialogueIndex != 8)
                NextDialogue();
        }
    }

    if (dialogueIndex != 8) return;

    for (auto key: PlayerInput::PlaySound) {
        if (hid->IsKeyDown(key.first)) {
            if(key.second > 1) return;
            sampleSources[key.second]->ForcePlaySound();
            for (int i = 0; i < sampleAnimators[key.second].size(); ++i) {
                sampleAnimators[key.second][i]->Reset();
            }

            if (key.second == 0) pattern++;
            else if (key.second == 1) {
                if (pattern == 2) {
                    for (int i = 0; i < 2; ++i) {
                        for (int j = 0; j < 2; ++j) {
                            GameObject::Destroy(sampleAnimators[i][j]->GetParent());
                        }
                    }
                    GameObject::Destroy(background->GetParent());
                    NextDialogue();
                }
                pattern = 0;
            }
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
    background.reset();
    for (int i = 0; i < 2; ++i) {
        if (!sampleSources.empty())
            sampleSources[i].reset();

        if (!sampleImages.empty())
            sampleImages[i].reset();

        if (!sampleAnimators.empty()) {
            for (int j = 0; j < 2; ++j) {
                if (!sampleAnimators[i].empty())
                    sampleAnimators[i][j].reset();
            }
        }
    }
    sampleAnimators.clear();
    clapIcon.reset();
    sampleSources.clear();
    instrumentControl.reset();
    cheatSheet.reset();
    texts.clear();
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