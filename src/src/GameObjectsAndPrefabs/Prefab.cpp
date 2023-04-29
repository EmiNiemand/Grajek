//
// Created by masterktos on 01.04.23.
//

#include "GameObjectsAndPrefabs/Prefab.h"

#include <utility>
#include "GameObjectsAndPrefabs/GameObject.h"

#include "EngineManagers/SceneManager.h"

#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/Animator.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/PlayerMovement.h"
#include "Components/Scripts/PlayerManager.h"
#include "Components/Scripts/PlayerEquipment.h"
#include "Components/Scripts/PlayerUI.h"
#include "Components/Scripts/Instrument.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Scripts/ShopMenu.h"
#include "Components/UI/Image.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/ShopTrigger.h"

std::shared_ptr<GameObject> Prefab::GetPlayer(std::string name) {
    auto player = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene);
    // Adding components
    auto playerRenderer = player->AddComponent<Renderer>();
    playerRenderer->LoadModel("domek/domek.obj");
    playerRenderer->material.reflection = 0.5f;
    auto cubeRigidbody = player->AddComponent<Rigidbody>();
    auto listener = player->AddComponent<AudioListener>();
    //Adding scripts
    player->AddComponent<PlayerManager>();
    // Setting values
    player->GetComponent<BoxCollider>()->SetOffset({0, 1, 0});
    player->transform->SetLocalPosition({0, 5, -10});
    player->transform->SetLocalScale({0.5, 1, 0.5});
    std::shared_ptr<GameObject> pivot = GameObject::Instantiate("Cube", player);
    pivot->transform->SetLocalPosition({0, 1, -10});

    return player;
}

std::shared_ptr<GameObject> Prefab::GetCube(std::string name) {
    auto sceneProp = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene, Tags::DEFAULT);
    auto scenePropRenderer = sceneProp->AddComponent<Renderer>();
    scenePropRenderer->LoadModel("domek/domek.obj");
    auto scenePropCollider = sceneProp->AddComponent<BoxCollider>();
    scenePropCollider->SetOffset({0, 1, 0});

    return sceneProp;
}

std::shared_ptr<GameObject> Prefab::GetDancingDude(std::string name) {
    auto animatedDood = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene, Tags::DEFAULT);
    auto animatedDoodAnimator = animatedDood->AddComponent<Animator>();
    animatedDoodAnimator->LoadAnimationModel("hiphopnigdystop/HipHopDancing.dae");
    animatedDoodAnimator->LoadAnimations("hiphopnigdystop/HipHopDancing.dae");

    return animatedDood;
}

std::shared_ptr<GameObject> Prefab::GetShop(std::string name) {
    auto shop = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene);
    shop->transform->SetLocalPosition({-10, 0, -5});
    shop->transform->SetLocalRotation({0, -45, 0});
    shop->transform->SetLocalScale({0.7, 0.7, 0.7});
    shop->AddComponent<Renderer>()->LoadModel("texturedModels/stoisko.obj");
    shop->AddComponent<BoxCollider>()->SetOffset({0, 2, -9});
    shop->GetComponent<BoxCollider>()->SetSize({1, 3, 3});

    auto shopTrigger = GameObject::Instantiate("ShopTrigger", shop);
    shopTrigger->AddComponent<BoxCollider>()->SetOffset({0, 2, -9});
    shopTrigger->GetComponent<BoxCollider>()->SetSize({4, 6, 6});
    shopTrigger->GetComponent<BoxCollider>()->isTrigger = true;
    shopTrigger->AddComponent<ShopTrigger>();
    GameObject::Instantiate("ShopButton", shopTrigger)->AddComponent<Image>()->LoadTexture(1600, 50, "UI/Sklep/Przycisk.png");
    shopTrigger->children.begin()->second->DisableSelfAndChildren();

    auto shopMenu = GameObject::Instantiate("ShopMenu", shop);
    auto shopMenuComponent = shopMenu->AddComponent<ShopMenu>();
    std::shared_ptr<GameObject> firstInstrument = shopMenuComponent->Menu::AddButton("FirstInstrument", 10, 0, "UI/Sklep/Perkusja.png", "UI/Sklep/PerkusjaZRamka.png");
    std::shared_ptr<GameObject> secondInstrument = shopMenuComponent->Menu::AddButton("SecondInstrument", 1425, 525, "UI/Sklep/Trabka.png", "UI/Sklep/TrabkaZRamka.png");
    std::shared_ptr<GameObject> thirdInstrument = shopMenuComponent->Menu::AddButton("ThirdInstrument", 1525, 250, "UI/Sklep/LaunbhPad.png", "UI/Sklep/LaunbhPadZRamka.png");
    std::shared_ptr<GameObject> fourthInstrument = shopMenuComponent->Menu::AddButton("FourthInstrument", 600, 700, "UI/Sklep/Gitara.png", "UI/Sklep/GitaraZRamka.png");
    std::shared_ptr<GameObject> shopBackground = shopMenuComponent->Menu::AddImage("ShopBackground", 0, 0, "UI/Sklep/Sklep.png");
    firstInstrument->GetComponent<Button>()->previousButton = thirdInstrument->GetComponent<Button>();
    firstInstrument->GetComponent<Button>()->nextButton = fourthInstrument->GetComponent<Button>();
    secondInstrument->GetComponent<Button>()->previousButton = fourthInstrument->GetComponent<Button>();
    secondInstrument->GetComponent<Button>()->nextButton = thirdInstrument->GetComponent<Button>();
    thirdInstrument->GetComponent<Button>()->previousButton = secondInstrument->GetComponent<Button>();
    thirdInstrument->GetComponent<Button>()->nextButton = firstInstrument->GetComponent<Button>();
    fourthInstrument->GetComponent<Button>()->previousButton = firstInstrument->GetComponent<Button>();
    fourthInstrument->GetComponent<Button>()->nextButton = secondInstrument->GetComponent<Button>();
    shopMenu->DisableSelfAndChildren();

    return shop;
}

std::shared_ptr<Instrument> Prefab::GetInstrument(InstrumentName instrumentName) {
    auto instrument = std::make_shared<Instrument>();
    instrument->Setup(instrumentName);

    // Add samples
    // -----------
    switch (instrumentName) {
        case Clap:
            instrument->AddSamples({
                "res/sounds/direct/clap/clapWeak.wav",
                "res/sounds/direct/clap/clapStrong.wav"});
            break;
        case Drums:
            instrument->AddSamples({
                "res/sounds/direct/drums/hat.wav",
                "res/sounds/direct/drums/kick.wav",
                "res/sounds/direct/drums/snare.wav"});
            break;
        // TODO: actually implement these three
        case Trumpet:
            instrument->AddSamples({
               "res/sounds/direct/drums/hat.wav",
               "res/sounds/direct/drums/kick.wav",
               "res/sounds/direct/drums/snare.wav"});
            break;
        case Launchpad:
            instrument->AddSamples({
               "res/sounds/direct/drums/hat.wav",
               "res/sounds/direct/drums/kick.wav",
               "res/sounds/direct/drums/snare.wav"});
            break;
        case Guitar:
            instrument->AddSamples({
               "res/sounds/direct/drums/hat.wav",
               "res/sounds/direct/drums/kick.wav",
               "res/sounds/direct/drums/snare.wav"});
            break;
    }

    // Add patterns
    // ------------
    switch (instrumentName) {
        case Clap:
            instrument->GeneratePattern({
                // 1      *
                // 0  * *
                {0, 0}, {0, 0.5}, {1, 0.5}});
            break;
        case Drums:
            // 2      *
            // 1    *   *
            // 0  *
            instrument->GeneratePattern({
                {0, 0}, {1, 0.5}, {2, 0.5},
                {1, 0.5}});
            break;
        // TODO: actually implement these three
        case Trumpet:
            // 2       -
            // 1     -   --
            // 0  --
            instrument->GeneratePattern({
                {0, 0, 1.0}, {1, 0.5, 0.5},
                {2, 0.5, 0.5}, {1, 0.5, 1.0}});
            break;
        case Launchpad:
            // 2   * *
            // 1 *     *
            // 0 -------
            instrument->GeneratePattern({
                {0, 0, 2.0},
                {1, 0}, {2, 0.5}, {2, 0.5},
                {1, 0.5}});
            break;
        case Guitar:
            // 2   *   *
            // 1 *   *
            // 0 -------
            instrument->GeneratePattern({
                {0, 0, 2.0},
                {1, 0}, {2, 0.5},
                {1, 0.5}, {2, 0.5}});
            break;
    }

    return instrument;
}