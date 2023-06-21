#include "Game.h"
#include "GloomEngine.h"
#include "ProjectSettings.h"
#include "EngineManagers/SceneManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/CubeMap.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Scripts/Player/PlayerMovement.h"
#include "Components/Scripts/Menus/PauseMenu.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Renderers/Animator.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/Scripts/Menus/SavePointMenu.h"
#include "Components/Animations/GameObjectAnimator.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "GameObjectsAndPrefabs/Prefabs/Player.h"
#include "GameObjectsAndPrefabs/Prefabs/Die.h"
#include "GameObjectsAndPrefabs/Prefabs/Indicator.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/Scripts/Menus/Shopkeeper.h"
#include "EngineManagers/AIManager.h"
#include "Components/Scripts/Opponent.h"
#include "Components/Scripts/Instrument.h"
#include "Components/Scripts/Crowd.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

Game::Game() {
    activeCamera = Camera::activeCamera;
    activeScene = SceneManager::GetInstance()->activeScene;
}

Game::~Game() = default;

void Game::InitializeGame() const {
#ifdef DEBUG
    ZoneScopedNC("Game Init", 0xDC143C);
#endif
    // Set up camera
    // -------------
    std::shared_ptr<Camera> camera = activeCamera->AddComponent<Camera>();
    camera->cameraOffset = glm::vec3(0, 25, 25);

    // Set up cubemap
    // --------------
    auto sky = GameObject::Instantiate("CubeMap", activeScene);
    auto skyCubeMap = sky->AddComponent<CubeMap>();
    skyCubeMap->LoadTextures("skybox/");

    // Load all animations
    // -------------
    Animator::LoadAnimation("CrowdAnimations/Walk.dae");
    Animator::LoadAnimation("CrowdAnimations/Happy.dae");
    Animator::LoadAnimation("CrowdAnimations/Angry.dae");
    Animator::LoadAnimation("CrowdAnimations/Idle1.dae");
    Animator::LoadAnimation("CrowdAnimations/Idle3.dae");
    Animator::LoadAnimation("MainHero/MainHeroIdle.dae");
    Animator::LoadAnimation("MainHero/MainHeroRun.dae");
    Animator::LoadAnimation("MainHero/MainHeroClap.dae");
    Animator::LoadAnimation("MainHero/MainHeroTrumpet.dae");
    Animator::LoadAnimation("MainHero/MainHeroDrums.dae");

    // Set up player
    // -------------
    std::shared_ptr<GameObject> player = Prefab::Instantiate<Player>();

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = Prefab::Instantiate<Die>("Ground");
    ground->transform->SetLocalPosition({0, -4, 0});
    ground->transform->SetLocalScale({150, 2, 150});
    ground->GetComponent<Renderer>()->textScale = glm::vec2(150, 150);

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<DirectionalLight>();
    sun->transform->SetLocalPosition({20, 40, 20});
    sun->transform->SetLocalRotation({-50, 70, 0});


    // Enemies
    auto drummer = GameObject::Instantiate("DrumOpponent", activeScene);
    auto drummerAnimator = drummer->AddComponent<Animator>();
    drummerAnimator->LoadAnimationModel("Opponent/Drummer/MainDrummer/Drummer.dae");
    drummerAnimator->SetAnimation("MainHero/MainHeroDrums.dae");
    drummer->AddComponent<BoxCollider>()->SetSize({0.5, 1, 0.5});
    drummer->transform->SetLocalPosition(glm::vec3(-53, 0, 18));
    drummer->transform->SetLocalRotation(glm::vec3(0, 155, 0));
    drummer->transform->SetLocalScale(glm::vec3(0.5, 0.5, 0.5));

    auto drummerOpponentComponent = drummer->AddComponent<Opponent>();
    drummerOpponentComponent->name = "Matt Ronome";
    drummerOpponentComponent->Setup(Instrument::GetInstrument(InstrumentName::Drums),
                                    {{1, 0.5}, {0, 0.5}, {2, 0.5}, {1, 0.25}, {0, 0.25}, {1, 0.5}, {2, 0.5}},
                                    80.0f, 50, glm::vec3(1, 0, 0), PlayerBadges::DRUMS);


    auto trumpeter = GameObject::Instantiate("JazzOpponent", activeScene);
    auto trumpeterAnimator = trumpeter->AddComponent<Animator>();
    trumpeterAnimator->LoadAnimationModel("Opponent/Trumpeter/MainTrumpeter/Trumpeter.dae");
    trumpeterAnimator->SetAnimation("MainHero/MainHeroTrumpet.dae");
    trumpeter->AddComponent<BoxCollider>()->SetSize({0.5, 1, 0.5});
    trumpeter->transform->SetLocalPosition(glm::vec3(-87, 0, -42));
    trumpeter->transform->SetLocalRotation(glm::vec3(0, 45, 0));
    trumpeter->transform->SetLocalScale(glm::vec3(0.5, 0.5, 0.5));


    auto trumpeterOpponentComponent = trumpeter->AddComponent<Opponent>();
    trumpeterOpponentComponent->name = "Christopher Jazzus";
    trumpeterOpponentComponent->Setup(Instrument::GetInstrument(InstrumentName::Trumpet),
                                              {{0, 0.5}, {1, 1}, {2, 0.5}, {1, 1}, {2, 0.5}}, 80.0f, 200,
                                              glm::vec3(1, 0, 0), PlayerBadges::TRUMPET);


    auto opponent1 = GameObject::Instantiate("NormalOpponent", activeScene);
    auto opponent1Animator = opponent1->AddComponent<Animator>();
    opponent1Animator->LoadAnimationModel("Crowd/BasicMan003/BasicMan003.dae");
    opponent1Animator->SetAnimation("MainHero/MainHeroClap.dae");
    opponent1->AddComponent<BoxCollider>()->SetSize({0.5, 1, 0.5});
    opponent1->transform->SetLocalPosition(glm::vec3(12, 0, 7));
    opponent1->transform->SetLocalRotation(glm::vec3(0, 210, 0));
    opponent1->transform->SetLocalScale(glm::vec3(0.5, 0.5, 0.5));


    auto opponent1OpponentComponent = opponent1->AddComponent<Opponent>();
    opponent1OpponentComponent->name = "Benjamin Klapper";
    opponent1OpponentComponent->Setup(Instrument::GetInstrument(InstrumentName::Clap),
                                      {{0, 0.5}, {0, 1}, {1, 1}}, 65.0f, 15,
                                      glm::vec3(1, 1, 0));


    auto opponent2 = GameObject::Instantiate("NormalOpponent", activeScene);
    auto opponent2Animator = opponent2->AddComponent<Animator>();
    opponent2Animator->LoadAnimationModel("Crowd/BasicMan003/BasicMan003.dae");
    opponent2Animator->SetAnimation("MainHero/MainHeroClap.dae");
    opponent2->AddComponent<BoxCollider>()->SetSize({0.5, 1, 0.5});
    opponent2->transform->SetLocalPosition(glm::vec3(-36.5, 0, -26));
    opponent2->transform->SetLocalRotation(glm::vec3(0, 310, 0));
    opponent2->transform->SetLocalScale(glm::vec3(0.5, 0.5, 0.5));


    auto opponent2OpponentComponent = opponent2->AddComponent<Opponent>();
    opponent2OpponentComponent->name = "Patt Ernie";
    opponent2OpponentComponent->Setup(Instrument::GetInstrument(InstrumentName::Clap),
                                      {{0, 0.5}, {0, 1}, {1, 1}}, 70.0f, 30,
                                      glm::vec3(1, 1, 0));


    auto opponent3 = GameObject::Instantiate("NormalOpponent", activeScene);
    auto opponent3Animator = opponent3->AddComponent<Animator>();
    opponent3Animator->LoadAnimationModel("Opponent/Drummer/BasicDrummer001/Drummer.dae");
    opponent3Animator->SetAnimation("MainHero/MainHeroDrums.dae");
    opponent3->AddComponent<BoxCollider>()->SetSize({0.5, 1, 0.5});
    opponent3->transform->SetLocalPosition(glm::vec3(-75.5, 0, -65.5));
    opponent3->transform->SetLocalRotation(glm::vec3(0, 40, 0));
    opponent3->transform->SetLocalScale(glm::vec3(0.5, 0.5, 0.5));

    auto opponent3OpponentComponent = opponent3->AddComponent<Opponent>();
    opponent3OpponentComponent->name = "Sam Pelle";
    opponent3OpponentComponent->Setup(Instrument::GetInstrument(InstrumentName::Drums),
                                      {{0, 0.5}, {1, 0.5}, {2, 0.5}, {1, 0.5}, {2, 0.5}}, 70.0f, 25,
                                      glm::vec3(1, 1, 0));

    // Town people
    auto dialogue = GameObject::Instantiate("GateDialogue", activeScene);
    dialogue->transform->SetLocalPosition(glm::vec3(-1, 0, -27));
    dialogue->transform->SetLocalScale(glm::vec3(0.5f));
    dialogue->AddComponent<BoxCollider>()->SetSize({1, 1, 1});
    auto dialogueAnimator = dialogue->AddComponent<Animator>();
    dialogueAnimator->LoadAnimationModel("Crowd/BobTheBuilder/Builder.dae");
    dialogueAnimator->SetAnimation("CrowdAnimations/Idle3.dae");
    auto dialogueComponent = GameObject::Instantiate("Dialogue", dialogue)->AddComponent<Dialogue>();
    dialogueComponent->name = "Tru Bob";
    dialogueComponent->texts.push_back({{"Sorry buddy, cannot let you in - this place is under construction."},
                                                       {"Come back when a music competition begins."},
                                                       {""}});
    auto dialogueIndicator = Prefab::Instantiate<Indicator>("Indicator");
    dialogueIndicator->SetParent(dialogue);
    dialogueIndicator->transform->SetLocalPosition(glm::vec3(0, 6.5, 0));
    dialogueIndicator->transform->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));

    dialogue = GameObject::Instantiate("PatternDialogue1", activeScene);
    dialogue->transform->SetLocalPosition(glm::vec3(-23.5, 0, -8));
    dialogue->transform->SetLocalRotation(glm::vec3(0, 90, 0));
    dialogue->transform->SetLocalScale(glm::vec3(0.5f));
    dialogue->AddComponent<BoxCollider>()->SetSize({1, 1, 1});
    dialogueAnimator = dialogue->AddComponent<Animator>();
    dialogueAnimator->LoadAnimationModel("Crowd/BasicMan003/BasicMan003.dae");
    dialogueAnimator->SetAnimation("CrowdAnimations/Idle3.dae");
    dialogueComponent = GameObject::Instantiate("Dialogue", dialogue)->AddComponent<Dialogue>();
    dialogueComponent->name = "David Kafferdale";
    dialogueComponent->texts.push_back({{"[singing]"},
                               {"Smoke on the water... a fire in the sky!"},
                               {"Smoke on the water... burn it down!"}});
    dialogueIndicator = Prefab::Instantiate<Indicator>("Indicator");
    dialogueIndicator->SetParent(dialogue);
    dialogueIndicator->transform->SetLocalPosition(glm::vec3(0, 6.5, 0));
    dialogueIndicator->transform->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));

    dialogue = GameObject::Instantiate("Felynn Rutin", activeScene);
    dialogue->transform->SetLocalPosition(glm::vec3(-59, 0, 2.5));
    dialogue->transform->SetLocalScale(glm::vec3(0.5f));
    dialogue->transform->SetLocalRotation(glm::vec3(0, 355, 0));
    dialogue->AddComponent<BoxCollider>()->SetSize({1, 1, 1});
    dialogueAnimator = dialogue->AddComponent<Animator>();
    dialogueAnimator->LoadAnimationModel("Crowd/BasicMan001/BasicMan001.dae");
    dialogueAnimator->SetAnimation("CrowdAnimations/Idle3.dae");
    dialogueComponent = GameObject::Instantiate("Dialogue", dialogue)->AddComponent<Dialogue>();
    dialogueComponent->name = "Felynn Rutin";
    dialogueComponent->texts.push_back({{"Man, I met a busker the other day that was really annoying."},
                                        {"He just kept playing the same thing over and over again."},
                                        {"I left that area really quickly."}});
    dialogueIndicator = Prefab::Instantiate<Indicator>("Indicator");
    dialogueIndicator->SetParent(dialogue);
    dialogueIndicator->transform->SetLocalPosition(glm::vec3(0, 6.5, 0));
    dialogueIndicator->transform->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));

    dialogue = GameObject::Instantiate("Jimmy Bravo", activeScene);
    dialogue->transform->SetLocalPosition(glm::vec3(18, 0, -15));
    dialogue->transform->SetLocalRotation(glm::vec3(0, 315, 0));
    dialogue->transform->SetLocalScale(glm::vec3(0.5f));
    dialogue->AddComponent<BoxCollider>()->SetSize({1, 1, 1});
    dialogueAnimator = dialogue->AddComponent<Animator>();
    dialogueAnimator->LoadAnimationModel("Crowd/JimmyBravo/JimmyBravo.dae");
    dialogueAnimator->SetAnimation("CrowdAnimations/Idle1.dae");
    dialogueComponent = GameObject::Instantiate("Dialogue", dialogue)->AddComponent<Dialogue>();
    dialogueComponent->name = "Jimmy Bravo";
    dialogueComponent->texts.push_back({{"I just saw some dude trying to battle someone with drums by simply clapping!"},
                                        {"I mean, it was impressive attempt, but there was no way he was going to win."},
                                        {""}});
    dialogueIndicator = Prefab::Instantiate<Indicator>("Indicator");
    dialogueIndicator->SetParent(dialogue);
    dialogueIndicator->transform->SetLocalPosition(glm::vec3(0, 6.5, 0));
    dialogueIndicator->transform->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));

    dialogue = GameObject::Instantiate("BadgeDialogue", activeScene);
    dialogue->transform->SetLocalPosition(glm::vec3(-58, 0, -65.5));
    dialogue->transform->SetLocalScale(glm::vec3(0.5f));
    dialogue->transform->SetLocalRotation(glm::vec3(0, 295, 0));
    dialogue->AddComponent<BoxCollider>()->SetSize({1, 1, 1});
    dialogueAnimator = dialogue->AddComponent<Animator>();
    dialogueAnimator->LoadAnimationModel("Crowd/Joseph/Joseph.dae");
    dialogueAnimator->SetAnimation("CrowdAnimations/Idle3.dae");
    dialogueComponent = GameObject::Instantiate("Dialogue", dialogue)->AddComponent<Dialogue>();
    dialogueComponent->name = "Joseph Joe Starr";
    dialogueComponent->texts.push_back({{"OOOOOOOOOOOH MYYYYYYYYYYYYYYYYYY"},
                                        {"GOOOOOOOOOOOOOOOOOOOOOOOOOOOOOD!"},
                                        {"The contest is approaching..."}});
    dialogueIndicator = Prefab::Instantiate<Indicator>("Indicator");
    dialogueIndicator->SetParent(dialogue);
    dialogueIndicator->transform->SetLocalPosition(glm::vec3(0, 6.5, 0));
    dialogueIndicator->transform->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));

    auto shopkeeper = GameObject::Instantiate("Shopkeeper", activeScene);
    shopkeeper->transform->SetLocalPosition(glm::vec3(1.5f, 0, -3));
    shopkeeper->AddComponent<Shopkeeper>();

#ifdef DEBUG
    AIManager::GetInstance()->InitializeSpawner(1);
#else
    AIManager::GetInstance()->InitializeSpawner(50);
#endif

    std::shared_ptr<GameObject> crowd = GameObject::Instantiate("Crowd", SceneManager::GetInstance()->activeScene);
    crowd->AddComponent<Crowd::Crowd>();

    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    auto pauseMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Pause");
    if (pauseMenu)
        if (pauseMenu->GetComponent<PauseMenu>())
            shouldQuit = pauseMenu->GetComponent<PauseMenu>()->gameShouldExit;

    auto mainMenu = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu");
    if (mainMenu)
        if (mainMenu->GetComponent<MainMenu>())
            shouldQuit = mainMenu->GetComponent<MainMenu>()->gameShouldExit;


    return shouldQuit;
}
