#include "Game.h"
#include "GloomEngine.h"
#include "EngineManagers/SceneManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/CubeMap.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Lights/PointLight.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/Renderers/Lights/SpotLight.h"
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
#include "Components/Audio/AudioListener.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Scripts/Menus/SavePointMenu.h"
#include "Components/Animations/GameObjectAnimator.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "GameObjectsAndPrefabs/Prefabs/Player.h"
#include "GameObjectsAndPrefabs/Prefabs/Die.h"
#include "GameObjectsAndPrefabs/Prefabs/House.h"
#include "GameObjectsAndPrefabs/Prefabs/SavePoint.h"
#include "Components/Scripts/Menus/Dialogue.h"

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
    camera->cameraOffset = glm::vec3(0, 30, 30);

    // Set up cubemap
    // --------------
    auto sky = GameObject::Instantiate("CubeMap", activeScene);
    auto skyCubeMap = sky->AddComponent<CubeMap>();
    skyCubeMap->LoadTextures("skybox/");

    // Load all animations
    // -------------
    Animator::LoadAnimation("AnimsNew/Walk.dae");
    Animator::LoadAnimation("AnimsNew/Happy.dae");
    Animator::LoadAnimation("AnimsNew/Angry.dae");
    Animator::LoadAnimation("AnimsNew/Idle1.dae");
    Animator::LoadAnimation("AnimsNew/Idle3.dae");

    // Set up player
    // -------------
    std::shared_ptr<GameObject> player = Prefab::Instantiate<Player>();

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = Prefab::Instantiate<Die>("Ground");
    ground->transform->SetLocalPosition({0, -4, -10});
    ground->transform->SetLocalScale({40, 2, 40});
    ground->GetComponent<Renderer>()->textScale = glm::vec2(40, 40);

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<DirectionalLight>();
    sun->transform->SetLocalPosition({20, 40, 20});
    sun->transform->SetLocalRotation({-50, 70, 0});

    std::shared_ptr<GameObject> bench = GameObject::Instantiate("Bench", activeScene);
    bench->transform->SetLocalPosition({0, 0, -10});
    bench->transform->SetLocalRotation({0, -90, 0});
    bench->transform->SetLocalScale({0.5, 0.5, 0.5});
    bench->AddComponent<Renderer>()->LoadModel("texturedModels/lawka.obj");
    bench->AddComponent<BoxCollider>()->SetOffset({5, 1, -2.5});
    bench->GetComponent<BoxCollider>()->SetSize({2, 2, 3});
    auto portalRadioSound = bench->AddComponent<AudioSource>();
    portalRadioSound->LoadAudioData("res/sounds/positional/portal_radio.wav", AudioType::Positional);
    portalRadioSound->SetPositionOffset({1.3f, 0.0f, 3.0f});
    portalRadioSound->SetDistanceMode(AudioDistanceMode::Continuous);
    portalRadioSound->SetMaxDistance(20.0f);
    portalRadioSound->SetCone({0.0f, 0.0f, 1.0f}, {110.0f, 200.0f});
    portalRadioSound->IsLooping(true);
    portalRadioSound->PlaySound();

    std::shared_ptr<GameObject> bench2 = GameObject::Instantiate("Bench", activeScene);
    bench2->transform->SetLocalPosition({10, 0, 0});
    bench2->transform->SetLocalRotation({0, 180, 0});
    bench2->transform->SetLocalScale({0.5, 0.5, 0.5});
    bench2->AddComponent<Renderer>()->LoadModel("texturedModels/lawka.obj");
    bench2->AddComponent<BoxCollider>()->SetOffset({5, 1, -2.5});
    bench2->GetComponent<BoxCollider>()->SetSize({2, 2, 3});

    std::shared_ptr<GameObject> hydrant = GameObject::Instantiate("Hydrant", activeScene);
    hydrant->transform->SetLocalPosition({15, 0, -15});
    hydrant->transform->SetLocalRotation({0, -65, 0});
    hydrant->transform->SetLocalScale({0.5, 0.5, 0.5});
    hydrant->AddComponent<Renderer>()->LoadModel("texturedModels/hydrant.obj");

    auto savePoint1 = Prefab::Instantiate<SavePoint>();
    savePoint1->transform->SetLocalPosition({-15, 0, 10});
    savePoint1->transform->SetLocalScale({2.0, 2.0, 2.0});

    int x = 0;
    int y = 0;

    // Set up animated model
//    for (int i = 0; i < 100; ++i) {
//        std::shared_ptr<GameObject> animatedDood = GameObject::Instantiate("DOOD", SceneManager::GetInstance()->activeScene, Tags::DEFAULT);
//        auto animatedDoodAnimator = animatedDood->AddComponent<Animator>();
//        animatedDoodAnimator->LoadAnimationModel("AnimsNew/Walk.dae");
//        animatedDoodAnimator->SetAnimation("AnimsNew/Walk.dae");
//        if (i % 25 == 0) {
//            x = 0;
//            y++;
//        }
//        animatedDood->transform->SetLocalPosition({-12 + x, 0, 5 + 2 * y});
//        animatedDood->transform->SetLocalRotation({0, 0, 0});
//        animatedDood->transform->SetLocalScale({0.5, 0.5, 0.5});
//        x++;
//    }

    // Setup sklepikarz
    auto sklepikarz = GameObject::Instantiate("Sklepikarz", activeScene);
    sklepikarz->AddComponent<Rigidbody>()->enabled = false;
    auto collider = sklepikarz->GetComponent<BoxCollider>();
    collider->SetOffset({0, 1, 0});
    collider->SetSize({1, 2, 1});
    sklepikarz->transform->SetLocalPosition({-10, 0, -10});
    sklepikarz->transform->SetLocalScale({0.5, 0.5, 0.5});
    auto animatorObject = GameObject::Instantiate("AnimatorSklepikarz", sklepikarz);
    auto animator = animatorObject->AddComponent<Animator>();
    animator->LoadAnimationModel("JazzMan001/JazzMan001.dae");
    animator->SetAnimation("AnimsNew/Idle3.dae");
    auto sklepikarzDialogue = GameObject::Instantiate("SklepikarzDialogue", sklepikarz);
    auto dialogue = sklepikarzDialogue->AddComponent<Dialogue>();
    dialogue->texts.push_back({{"Jestem Sklepu."},
                              {"Mozesz sie poruszac WSAD"},
                                    {"Kup instrument."}});
    dialogue->texts.push_back({{"Graj spacja."},
                               {"Strzelaj przyciskami RUP"},
                               {"Rozwalaj wrogow."}});

//    // SCENE BUILDINGS
//	std::map<std::string, int> buildingSizes = {
//			{"jazz1", 6},
//			{"jazz2", 7},
//			{"jazz3", 10},
//			{"jazz4", 6},
//			{"kamienica1", 6},
//			{"kamienica2", 10},
//			{"kamienica3", 6},
//			{"moduê1", 6},
//			{"moduê2", 6},
//			{"moduê3", 6},
//			{"moduê4", 6},
//			{"moduê5", 10},
//			{"moduê6", 6}
//	};
//	float currentXPos = -25;
//	float currentYPos = -30;
//
//	std::string squareBuildings[] = {
//			"kamienica1", "kamienica2", "kamienica3",
//			"moduê1", "moduê2"
//	};
//
//	std::vector<std::string> buildingPaths = {
//			squareBuildings[0], squareBuildings[2],
//			squareBuildings[0], squareBuildings[4], squareBuildings[1],
//			squareBuildings[3], squareBuildings[2],
//	};
//
//	// LEFT CORNER
//	{
//		std::shared_ptr<GameObject> test = GameObject::Instantiate("TestHouse", activeScene);
//		test->transform->SetLocalPosition({currentXPos - 9.5, 0, currentYPos+4});
//		test->transform->SetLocalRotation({0, 90, 0});
//		test->AddComponent<Renderer>()->LoadModel("Budynki/modele/moduê5.obj");
//		test->AddComponent<BoxCollider>()->SetOffset({0, 3, 0});
//		test->GetComponent<BoxCollider>()->SetSize({buildingSizes["moduê5"], 6, 3});
//	}
//
//	// FRONT FACE
//	for(int i=0; i < buildingPaths.size(); i++) {
//		currentXPos += buildingSizes[buildingPaths[i]]/2.0f;
//
//		std::shared_ptr<GameObject> test = GameObject::Instantiate("TestHouse", activeScene);
//		test->transform->SetLocalPosition({currentXPos, 0, currentYPos});
//		test->AddComponent<Renderer>()->LoadModel("Budynki/modele/"+buildingPaths[i]+".obj");
//		test->AddComponent<BoxCollider>()->SetOffset({0, 3, 0});
//		test->GetComponent<BoxCollider>()->SetSize({buildingSizes[buildingPaths[i]], 6, 3});
//
//		currentXPos += buildingSizes[buildingPaths[i]]/2.0f;
//	}
//
//	// RIGHT CORNER
//	{
//		std::shared_ptr<GameObject> test = GameObject::Instantiate("TestHouse", activeScene);
//		test->transform->SetLocalPosition({currentXPos + 5, 0, currentYPos});
//		test->AddComponent<Renderer>()->LoadModel("Budynki/modele/moduê5.obj");
//		test->AddComponent<BoxCollider>()->SetOffset({0, 3, 0});
//		test->GetComponent<BoxCollider>()->SetSize({buildingSizes["moduê5"], 6, 3});
//	}
//
//	currentYPos = -20;
//
//	// LEFT FACE
//	for(int i=0; i < buildingPaths.size(); i++) {
//		currentYPos += buildingSizes[buildingPaths[i]]/2.0f;
//
//		std::shared_ptr<GameObject> test = GameObject::Instantiate("TestHouse", activeScene);
//		test->transform->SetLocalPosition({-35, 0, currentYPos});
//		test->transform->SetLocalRotation({0, 90, 0});
//		test->AddComponent<Renderer>()->LoadModel("Budynki/modele/"+buildingPaths[i]+".obj");
//		test->AddComponent<BoxCollider>()->SetOffset({0, 3, 0});
//		test->GetComponent<BoxCollider>()->SetSize({buildingSizes[buildingPaths[i]], 6, 3});
//
//		currentYPos += buildingSizes[buildingPaths[i]]/2.0f;
//	}
//
//	currentYPos = -20;
//
//	// RIGHT FACE
//	for(int i=0; i < buildingPaths.size(); i++) {
//		currentYPos += buildingSizes[buildingPaths[i]]/2.0f;
//
//		std::shared_ptr<GameObject> test = GameObject::Instantiate("TestHouse", activeScene);
//		test->transform->SetLocalPosition({currentXPos+10, 0, currentYPos});
//		test->transform->SetLocalRotation({0, -90, 0});
//		test->AddComponent<Renderer>()->LoadModel("Budynki/modele/"+buildingPaths[i]+".obj");
//		test->AddComponent<BoxCollider>()->SetOffset({0, 3, 0});
//		test->GetComponent<BoxCollider>()->SetSize({buildingSizes[buildingPaths[i]], 6, 3});
//
//		currentYPos += buildingSizes[buildingPaths[i]]/2.0f;
//	}

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("Pause"))
        if (GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>())
            shouldQuit = GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->gameShouldExit;

    if (GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu"))
        if (GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu")->GetComponent<MainMenu>())
            shouldQuit = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu")->GetComponent<MainMenu>()->gameShouldExit;


    return shouldQuit;
}