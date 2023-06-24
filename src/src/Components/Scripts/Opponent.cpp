#include <utility>
#include "GloomEngine.h"
#include "Components/Scripts/Opponent.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GameObjectsAndPrefabs/Prefabs/Indicator.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/HIDManager.h"
#include "EngineManagers/DialogueManager.h"
#include "Components/Scripts/Player/PlayerEquipment.h"
#include "EngineManagers/AIManager.h"
#include "Components/Scripts/SessionUI/SessionUI.h"
#include "Components/Scripts/Crowd.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/Scripts/Player/PlayerInput.h"

Opponent::Opponent(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Opponent::~Opponent() = default;

void Opponent::Setup(std::shared_ptr<Instrument> instrument1, float patterDelay1, float accuracy1, short bet1,
                     glm::vec3 indicatorColor, PlayerBadges badge1) {
//     Setup pattern
    sampleSources.reserve(5);
    instrument = std::move(instrument1);
    pattern = instrument->opponentPatterns[RandomnessManager::GetInstance()->GetInt(0,
                                           (int)instrument->opponentPatterns.size() - 1)];

    for (const auto& sound: pattern->sounds) {
        sampleSources.push_back(GameObject::Instantiate("OpponentSampleSource", parent)->AddComponent<AudioSource>());
        auto sample = sampleSources.back();
        sample->LoadAudioData(sound->sample->clipPath, AudioType::Positional);
        sample->SetDistanceMode(AudioDistanceMode::Continuous);
        sample->SetPitch(0.9f);
        sample->SetMaxDistance(10);
    }

    accuracy = accuracy1;
    bet = bet1;
    badge = badge1;
    patternDelay = patterDelay1;

//     Setup UI
    ui = GameObject::Instantiate("OpponentUI", parent);
    GameObject::Instantiate("OpponentFrame", ui)->AddComponent<Image>()
            ->LoadTexture(460, 880, "UI/Opponent/battleFrame.png", -0.5);
    GameObject::Instantiate("OpponentFrameBG", ui)->AddComponent<Image>()
            ->LoadTexture(460, 880, "UI/Opponent/battleFrameBG.png", -0.4);
    belt = GameObject::Instantiate("OpponentSatisfactionDifference", ui)->AddComponent<Image>();
    belt->LoadTexture(960, 920, "UI/Opponent/progressBar.png", -0.45);
    belt->SetScale(0.0f);
    timeCounter = GameObject::Instantiate("OpponentTimeCounter", ui)->AddComponent<Image>();
    timeCounter->LoadTexture(468, 1036, "UI/Opponent/timeBar.png", -0.55);
    ui->DisableSelfAndChildren();

    indicator = Prefab::Instantiate<Indicator>("Indicator");
    indicator->SetParent(parent);
    indicator->transform->SetLocalPosition(glm::vec3(0, 6.5, 0));
    indicator->transform->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));
    indicator->GetComponent<Renderer>()->material.color = indicatorColor;

//     Setup dialogues
    dialogue = GameObject::Instantiate("OpponentDialogue", parent)->AddComponent<Dialogue>();
    winDialogue = GameObject::Instantiate("OpponentWinDialogue", parent)->AddComponent<Dialogue>();
    winDialogue->forced = true;
    lossDialogue = GameObject::Instantiate("OpponentLossDialogue", parent)->AddComponent<Dialogue>();
    lossDialogue->forced = true;
    dialogue->texts.push_back({{"Yo, wanna see, who plays " + instrument->NameToString() + " better?"},
                               {"Then why don't we make a little bet to spice things up!"},
                               {""}});
    dialogue->texts.push_back({{"Let's say, whoever wins, gets " + std::to_string(bet)},
                               {""},
                               {""}});
    dialogue->texts.push_back({{""},{""},{""}});
    winDialogue->texts.push_back({{"That was fantastic! Thanks for playing with me bud."},
                                  {std::to_string(bet) + " well earned. Good luck with other buskers!"},
                                  {""}});
    lossDialogue->texts.push_back({{"Unfortunately, you need a bit more practice bud."},
                                   {"Definitely come back later though! You have a lot of potential."},
                                   {"I feel that our battle could be legendary."}});
    dialogue->name = name;
    winDialogue->name = name;
    lossDialogue->name = name;

//     Setup choose menu
    chooseMenu = GameObject::Instantiate("OpponentChooseMenu", parent);
    chooseMenu->AddComponent<Image>()->LoadTexture(700, 420, "UI/Opponent/chooseMenu.png", -0.6f);
    button1 = GameObject::Instantiate("OpponentChooseMenuButton1", chooseMenu)->AddComponent<Button>();
    button1->LoadTexture(772, 450, "UI/Opponent/acceptBattle.png", "UI/Opponent/acceptBattleActive.png", -0.65f);
    button2 = GameObject::Instantiate("OpponentChooseMenuButton2", chooseMenu)->AddComponent<Button>();
    button2->LoadTexture(1016, 450, "UI/Opponent/declineBattle.png", "UI/Opponent/declineBattleActive.png", -0.65f);
    button2->isActive = true;
    GameObject::Instantiate("OpponentChooseMenuImage", chooseMenu)->AddComponent<Image>()->
            LoadTexture(0, 0, "UI/backgroundOpacity60.png", 0.65f);
    chooseMenu->DisableSelfAndChildren();

    // Setup sounds
    winSound = GameObject::Instantiate("WinAudioSource", parent)->AddComponent<AudioSource>();
    winSound->LoadAudioData("res/sounds/direct/win.wav", AudioType::Direct);
    loseSound = GameObject::Instantiate("LoseAudioSource", parent)->AddComponent<AudioSource>();
    loseSound->LoadAudioData("res/sounds/direct/lose.wav", AudioType::Direct);
}

void Opponent::Start() {
    dialogue->enabled = false;
    playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    auto playerEquipment = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerEquipment>();
    if (playerEquipment->badges.contains(badge) && playerEquipment->badges[badge]) {
        defeated = true;
        indicator->GetComponent<Renderer>()->material.color = glm::vec3(1, 1, 1);
    }
    if (badge == PlayerBadges::DRUMS) {
        dialogue->dialogueImage->LoadTexture(127, 0, "UI/Dialogues/Dialog2.png");
        winDialogue->dialogueImage->LoadTexture(127, 0, "UI/Dialogues/Dialog2.png");
        lossDialogue->dialogueImage->LoadTexture(127, 0, "UI/Dialogues/Dialog2.png");
    } else if (badge == PlayerBadges::TRUMPET) {
        dialogue->dialogueImage->LoadTexture(127, 0, "UI/Dialogues/Dialog4.png");
        winDialogue->dialogueImage->LoadTexture(127, 0, "UI/Dialogues/Dialog4.png");
        lossDialogue->dialogueImage->LoadTexture(127, 0, "UI/Dialogues/Dialog4.png");
    } else {
        winDialogue->dialogueImage->LoadTexture(127, 0, dialogue->dialogueImage->filePath);
        lossDialogue->dialogueImage->LoadTexture(127, 0, dialogue->dialogueImage->filePath);
    }

    timer = pattern->sounds[sampleIndex]->duration * 60 / (float)instrument->genre;
    sampleSources[sampleIndex]->ForcePlaySound();

    Component::Start();
}

void Opponent::Update() {

    float deltaTime = GloomEngine::GetInstance()->deltaTime;
    if (musicSession && !shouldPlay) {
        startPlayingTimer -= deltaTime;
        if (startPlayingTimer <= 0) {
            shouldPlay = true;
        }
    }

    if (shouldPlay) {
        patternTimer -= deltaTime;
        if (patternTimer <= 0) {
            // Play pattern
            timer += deltaTime;
            if (timer >= (pattern->sounds[sampleIndex]->delay + pattern->sounds[sampleIndex]->delay *
            (1 - accuracy / 100)) * 60 / (float)instrument->genre) {
                if (sampleIndex > 0 && instrument->name == InstrumentName::Trumpet) sampleSources[sampleIndex - 1]->StopSound();

                if (sampleIndex == pattern->sounds.size() - 1) {

                    sampleSources[sampleIndex]->StopSound();

                    if (musicSession) {
                        AIManager::GetInstance()->NotifyOpponentPlayedPattern(pattern);
                        float diff = AIManager::GetInstance()->GetCombinedOpponentSatisfaction(accuracy, (int)pattern->sounds.size());
                        satisfactionDifference -= diff * 2;
                        belt->SetScale(glm::vec2(satisfactionDifference / 100.0f, 1.0f));
                    }

                    pattern = instrument->opponentPatterns[RandomnessManager::GetInstance()->GetInt(0,
                                                           (int)instrument->opponentPatterns.size() - 1)];
                    for (const auto& sample : sampleSources) {
                        GameObject::Destroy(sample->GetParent());
                    }

                    sampleSources.clear();

                    for (const auto& sound: pattern->sounds) {
                        sampleSources.push_back(GameObject::Instantiate("OpponentSampleSource", parent)->AddComponent<AudioSource>());
                        auto sample = sampleSources.back();
                        sample->LoadAudioData(sound->sample->clipPath, AudioType::Positional);
                        sample->SetDistanceMode(AudioDistanceMode::Continuous);
                        sample->SetPitch(0.9f);
                        sample->SetMaxDistance(10);
                    }

                    patternTimer = patternDelay;
                    sampleIndex = -1;
                }
                ++sampleIndex;
                sampleSources[sampleIndex]->ForcePlaySound();
                timer = 0.0f - (pattern->sounds[sampleIndex]->duration + pattern->sounds[sampleIndex]->duration *
                        (1 - accuracy / 100)) * 60 / (float)instrument->genre;
            }
        }
    }


    if (!dialogue->triggerActive) return;

    if(dialogue->menuActive) return;

    auto hid = HIDManager::GetInstance();


    for (const auto& interactKey : PlayerInput::Interact) {
        if (hid->IsKeyDown(interactKey.first) && defeated) {
            winDialogue->ShowDialogue();
            return;
        }

        // Show first dialogue
        if (hid->IsKeyDown(interactKey.first) && !dialogueActive && !sessionStarter && !musicSession && !lossDialogue->active && !winDialogue->active) {
            dialogueActive = true;
            shouldPlay = false;
            dialogue->ShowDialogue();
            AIManager::GetInstance()->NotifyPlayerTalksWithOpponent(true);
            return;
        }
    }

    for (const auto& applyKey : PlayerInput::Apply) {
        // Hide win dialogue
        if (hid->IsKeyDown(applyKey.first) && winDialogue->active && winDialogue->dialogueIndex) {
            dialogue->image->enabled = true;
            winDialogue->HideDialogue();
            return;
        }

        // Show choose menu
        if (hid->IsKeyDown(applyKey.first) && dialogueActive && !chooseMenuActive && !rejectDialogueActive && !acceptDialogueActive) {
            chooseMenuActive = true;
            button1->isActive = false;
            button2->isActive = true;
            chooseMenu->EnableSelfAndChildren();
            dialogue->NextDialogue();
            dialogue->texts[1].text1 = "Let's say, whoever wins, gets " + std::to_string(bet);
            dialogue->texts[1].text2 = "";
            return;
        }

        // Choose menu
        if (chooseMenuActive) {
            for (const auto& moveKey : PlayerInput::Move) {
                if (!hid->IsKeyDown(moveKey.first)) continue;
                if (moveKey.second == 3) {
                    button1->isActive = true;
                    button2->isActive = false;
                    return;
                }
                if (moveKey.second == 1) {
                    button1->isActive = false;
                    button2->isActive = true;
                    return;
                }
            }

            if (hid->IsKeyDown(applyKey.first)) {
                chooseMenuActive = false;
                chooseMenu->DisableSelfAndChildren();
                dialogue->image->enabled = false;
                if (button1->isActive) {
                    if (playerManager->GetCash() < bet) {
                        dialogue->texts[2].text1 = "Sorry, but you don't have enough money.";
                        dialogue->texts[2].text2 = "Come back when you'll have at leat " + std::to_string(bet) + ".";
                        AIManager::GetInstance()->NotifyPlayerTalksWithOpponent(false);
                        rejectDialogueActive = true;
                    } else {
                        dialogue->texts[2].text1 = "Alright then, let's go!";
                        dialogue->texts[2].text2 = "";
                        acceptDialogueActive = true;
                    }
                } else {
                    dialogue->texts[2].text1 = "Understandable, have a great day!";
                    dialogue->texts[2].text2 = "";
                    AIManager::GetInstance()->NotifyPlayerTalksWithOpponent(false);
                    rejectDialogueActive = true;
                }
                dialogue->NextDialogue();
                return;
            }
        }

        // Hide reject dialogue
        if (hid->IsKeyDown(applyKey.first) && rejectDialogueActive) {
            dialogue->enabled = false;
            dialogue->HideDialogue();
            dialogueActive = false;
            rejectDialogueActive = false;
            dialogue->image->enabled = true;
            return;
        }

        // Hide accept dialogue
        if (hid->IsKeyDown(applyKey.first) && acceptDialogueActive) {
            dialogue->enabled = false;
            dialogue->HideDialogue();
            dialogueActive = false;
            acceptDialogueActive = false;
            dialogue->image->enabled = false;
            sessionStarter = true;
            playerManager->StartSessionWithOpponent(std::dynamic_pointer_cast<Opponent>(shared_from_this()));
            return;
        }

        // Music session
        if (musicSession) {
            time += deltaTime;
            timeCounter->SetScale(glm::vec2(1 - time / battleTime, 1));
            if (time >= battleTime || satisfactionDifference >= 100 || satisfactionDifference <= -100) {
                if (satisfactionDifference <= -100 || (time >= battleTime && satisfactionDifference <= 0)) {
                    lossDialogue->ShowDialogue();
                    time = 0.0f;
                    satisfactionDifference = 0.0f;
                    playerManager->EndSessionWithOpponent(false, bet);
                    loseSound->PlaySound();
                }
                if (satisfactionDifference >= 100 || (time >= battleTime && satisfactionDifference > 0)) {
                    defeated = true;
                    winDialogue->ShowDialogue();
                    playerManager->EndSessionWithOpponent(true, bet);
                    if (badge != (PlayerBadges)(-1)) playerManager->ReceiveBadge(badge);
                    indicator->GetComponent<Renderer>()->material.color = glm::vec3(1, 1, 1);
                    auto crowd = GloomEngine::GetInstance()->FindGameObjectWithName("Crowd");
                    if (crowd) crowd->GetComponent<Crowd::Crowd>()->OnEnemyDefeat(badge);
                    winSound->PlaySound();
                }
                AIManager::GetInstance()->NotifyPlayerTalksWithOpponent(false);
                ui->DisableSelfAndChildren();
                DialogueManager::GetInstance()->NotifyMenuIsNotActive();
                dialogue->image->enabled = false;
                musicSession = false;
                AIManager::GetInstance()->NotifyOpponentStopsPlaying();
            }
            return;
        }

        // Hide lose dialogue
        if (hid->IsKeyDown(applyKey.first) && lossDialogue->active) {
            dialogue->image->enabled = true;
            lossDialogue->HideDialogue();
            return;
        }
    }

    Component::Update();
}

void Opponent::PlayerPlayedPattern(float playerSatisfaction) {
    shouldPlay = true;
    float diff = playerSatisfaction;
    satisfactionDifference += diff * 2;
    belt->SetScale(glm::vec2(satisfactionDifference / 100.0f, 1.0f));
}

void Opponent::OnDestroy() {
    ui.reset();
    instrument.reset();
    pattern.reset();
    sampleSources.clear();
    playerManager.reset();
    button1.reset();
    button2.reset();
    chooseMenu.reset();
    belt.reset();
    dialogue.reset();
    winDialogue.reset();
    lossDialogue.reset();
    timeCounter.reset();
    Component::OnDestroy();
}

void Opponent::PlayerStartedMusicSession() {
    musicSession = true;
    sessionStarter = false;
    belt->SetScale(0);
    timeCounter->SetScale(1);
    ui->EnableSelfAndChildren();
    dialogue->menuActive = false;
    AIManager::GetInstance()->NotifyOpponentStartsPlaying(instrument->name, instrument->genre);
}