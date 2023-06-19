#include "GloomEngine.h"
#include "Components/Scripts/Opponent.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GameObjectsAndPrefabs/Prefabs/ConeIndicator.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/HIDManager.h"
#include "Components/UI/Button.h"
#include "EngineManagers/DialogueManager.h"
#include "Components/Scripts/Player/PlayerEquipment.h"
#include "EngineManagers/AIManager.h"
#include "Components/Scripts/SessionUI/SessionUI.h"

Opponent::Opponent(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Opponent::~Opponent() = default;

void Opponent::Setup(std::shared_ptr<Instrument> instrument1, std::vector<RawSample> musicPattern, float satisfaction1,
                     short bet1, glm::vec3 indicatorColor, PlayerBadges badge1) {
    // Setup pattern
    instrument = std::move(instrument1);
    instrument->GeneratePattern(musicPattern);
    pattern = instrument->patterns.back();
    pattern->sounds[0]->delay = musicPattern[0].delay;
    for (const auto& sound: instrument->patterns.back()->sounds) {
        sampleSources.push_back(GameObject::Instantiate("OpponentSampleSource", parent)->AddComponent<AudioSource>());
        auto sample = sampleSources.back();
        sample->LoadAudioData(sound->sample->clipPath, AudioType::Positional);
        sample->SetDistanceMode(AudioDistanceMode::Continuous);
        sample->SetMaxDistance(10);
        if (instrument->name == InstrumentName::Clap) sample->SetGain(0.25f);
    }

    satisfaction = satisfaction1;
    bet = bet1;
    badge = badge1;

    // Setup UI
    ui = GameObject::Instantiate("OpponentUI", parent);
    GameObject::Instantiate("OpponentFrame", ui)->AddComponent<Image>()->LoadTexture(460, 880, "UI/RamkaPrzeciwnik.png");
    belt = GameObject::Instantiate("OpponentSatisfactionDifference", ui)->AddComponent<Image>();
    belt->LoadTexture(960, 880, "UI/Satisfaction.png");
    belt->SetScale(0.0f);
    timeCounter = GameObject::Instantiate("OpponentTimeCounter", ui)->AddComponent<Image>();
    timeCounter->LoadTexture(460, 865, "UI/TimeCounter.png");
    ui->DisableSelfAndChildren();

    indicator = Prefab::Instantiate<ConeIndicator>("Indicator");
    indicator->SetParent(parent);
    indicator->transform->SetLocalPosition(glm::vec3(0, 6.5, 0));
    indicator->transform->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));
    indicator->GetComponent<Renderer>()->material.color = indicatorColor;

    // Setup dialogues
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

    // Setup choose menu
    chooseMenu = GameObject::Instantiate("OpponentChooseMenu", parent);
    chooseMenu->AddComponent<Image>()->LoadTexture(560, 290, "UI/ChooseMenu.png", -0.6f);
    button1 = GameObject::Instantiate("OpponentChooseMenuButton1", chooseMenu)->AddComponent<Button>();
    button1->LoadTexture(700, 400, "UI/buttonInactive.png", "UI/buttonActive.png", -0.65f);
    button1->LoadFont("Yes", 32);
    button2 = GameObject::Instantiate("OpponentChooseMenuButton2", chooseMenu)->AddComponent<Button>();
    button2->LoadTexture(1050, 400, "UI/buttonInactive.png", "UI/buttonActive.png", -0.65f);
    button2->LoadFont("No", 32);
    button2->isActive = true;
    chooseMenu->DisableSelfAndChildren();
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
    Component::Start();
}

void Opponent::Update() {
    // Play pattern
    timer += GloomEngine::GetInstance()->deltaTime;
    if (timer >= pattern->sounds[sampleIndex]->delay) {
        timer = 0.0f;
        if (sampleIndex > 0 && instrument->name == InstrumentName::Trumpet) sampleSources[sampleIndex - 1]->StopSound();
        sampleSources[sampleIndex]->ForcePlaySound();
        sampleIndex++;
        if (sampleIndex >= pattern->sounds.size())
            sampleIndex = 0;
    }

    if (!dialogue->triggerActive) return;

    if(dialogue->menuActive) return;

    auto hid = HIDManager::GetInstance();

    if (hid->IsKeyDown(Key::KEY_E) && defeated) {
        winDialogue->ShowDialogue();
        return;
    }

    // Hide win dialogue
    if (hid->IsKeyDown(Key::KEY_ENTER) && winDialogue->active && winDialogue->dialogueIndex) {
        dialogue->image->enabled = true;
        winDialogue->HideDialogue();
    }

    // Show first dialogue
    if (hid->IsKeyDown(Key::KEY_E) && !dialogueActive && !sessionStarter && !musicSession && !lossDialogue->active && !winDialogue->active) {
        dialogueActive = true;
        dialogue->ShowDialogue();
        AIManager::GetInstance()->NotifyPlayerTalksWithOpponent(true);
        return;
    }

    // Show choose menu
    if (hid->IsKeyDown(Key::KEY_ENTER) && dialogueActive && !chooseMenuActive && !rejectDialogueActive && !acceptDialogueActive) {
        chooseMenuActive = true;
        button1->isActive = false;
        button2->isActive = true;
        chooseMenu->EnableSelfAndChildren();
        //DialogueManager::GetInstance()->NotifyMenuIsActive();
        dialogue->NextDialogue();
        dialogue->texts[1].text1 = "Let's say, whoever wins, gets " + std::to_string(bet);
        dialogue->texts[1].text2 = "";
        return;
    }

    // Choose menu
    if (chooseMenuActive) {
        if (hid->IsKeyDown(Key::KEY_A) || hid->IsKeyDown(Key::KEY_ARROW_LEFT)) {
            button1->isActive = true;
            button2->isActive = false;
            return;
        }
        if (hid->IsKeyDown(Key::KEY_D) || hid->IsKeyDown(Key::KEY_ARROW_RIGHT)) {
            button1->isActive = false;
            button2->isActive = true;
            return;
        }
        if (hid->IsKeyDown(Key::KEY_ENTER)) {
            chooseMenuActive = false;
            chooseMenu->DisableSelfAndChildren();
            //DialogueManager::GetInstance()->NotifyMenuIsNotActive();
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
    if (hid->IsKeyDown(Key::KEY_ENTER) && rejectDialogueActive) {
        dialogue->enabled = false;
        dialogue->HideDialogue();
        dialogueActive = false;
        rejectDialogueActive = false;
        dialogue->image->enabled = true;
        return;
    }

    // Hide accept dialogue
    if (hid->IsKeyDown(Key::KEY_ENTER) && acceptDialogueActive) {
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
        time += GloomEngine::GetInstance()->deltaTime;
        timeCounter->SetScale(glm::vec2(1 - time / battleTime, 1));
        if (time >= battleTime || satisfactionDifference >= 100 || satisfactionDifference <= -100) {
            if (satisfactionDifference <= -100 || (time >= battleTime && satisfactionDifference <= 0)) {
                lossDialogue->ShowDialogue();
                time = 0.0f;
                satisfactionDifference = 0.0f;
                playerManager->EndSessionWithOpponent(false, bet);
            }
            if (satisfactionDifference >= 100 || (time >= battleTime && satisfactionDifference > 0)) {
                defeated = true;
                winDialogue->ShowDialogue();
                playerManager->EndSessionWithOpponent(true, bet);
                if (badge != (PlayerBadges)(-1)) playerManager->ReceiveBadge(badge);
                indicator->GetComponent<Renderer>()->material.color = glm::vec3(1, 1, 1);
                // TODO add sound when player beat boss
            }
            AIManager::GetInstance()->NotifyPlayerTalksWithOpponent(false);
            ui->DisableSelfAndChildren();
            DialogueManager::GetInstance()->NotifyMenuIsNotActive();
            dialogue->image->enabled = false;
            musicSession = false;
        }
        return;
    }

    // Hide lose dialogue
    if (hid->IsKeyDown(Key::KEY_ENTER) && lossDialogue->active) {
        dialogue->image->enabled = true;
        lossDialogue->HideDialogue();
    }

    Component::Update();
}

void Opponent::PlayerPlayedPattern(float satisfaction1) {
    float s = satisfaction1 - satisfaction;
    satisfactionDifference += s;
    belt->SetScale(glm::vec2(satisfactionDifference / 100, 1.0f));
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
}