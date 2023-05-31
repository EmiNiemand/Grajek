#include "GloomEngine.h"
#include "Components/Scripts/Opponent.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/OpponentManager.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/HIDManager.h"
#include "Components/UI/Button.h"
#include "EngineManagers/DialogueManager.h"
#include "Components/Scripts/OpponentSessionStarter.h"
#include "Components/Scripts/Player/PlayerEquipment.h"

Opponent::Opponent(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Opponent::~Opponent() = default;

void Opponent::Setup(std::shared_ptr<Instrument> instrument1, std::vector<RawSample> musicPattern, float satisfaction1) {
    // Setup pattern
    instrument = std::move(instrument1);
    instrument->GeneratePattern(musicPattern);
    pattern = instrument->patterns.back();
    pattern->sounds[0]->delay = musicPattern[0].delay;
    for (const auto& sound: instrument->patterns.back()->sounds) {
        sampleSources.push_back(GameObject::Instantiate("OpponentSampleSource", parent)->AddComponent<AudioSource>());
        auto sample = sampleSources.back();
        sample->LoadAudioData(sound->sample->clipPath.c_str(), AudioType::Positional);
        sample->SetPositionOffset(parent->parent->transform->GetLocalPosition());
    }
    satisfaction = satisfaction1;

    // Setup UI
    ui = GameObject::Instantiate("OpponentUI", parent);
    auto indicator = GameObject::Instantiate("OpponentSatisfaction", ui)->AddComponent<Image>();
    indicator->LoadTexture(700 + (int)satisfaction * 5, 1000, "UI/satysfakcjaPrzeciwnika.png");
    indicator->SetScale(0.5);
    auto belt1 = GameObject::Instantiate("OpponentAverageSatisfaction", ui);
    for (int i = 0; i < 20; i++) {
        belt.push_back(GameObject::Instantiate("OpponentAverageSatisfaction", belt1)->AddComponent<Image>());
        belt.back()->LoadTexture(700 + i * 25, 1000, "UI/satysfakcjaGracza.png");
        belt.back()->SetScale(0.5f);
    }
    ui->DisableSelfAndChildren();

    // Setup dialogues
    dialogue = GameObject::Instantiate("OpponentDialogue", parent)->AddComponent<Dialogue>();
    winDialogue = GameObject::Instantiate("OpponentWinDialogue", parent)->AddComponent<Dialogue>();
    winDialogue->forced = true;
    lossDialogue = GameObject::Instantiate("OpponentLossDialogue", parent)->AddComponent<Dialogue>();
    lossDialogue->forced = true;

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
    Component::Start();
}

void Opponent::Update() {
    if (defeated) return;

    // Play pattern
    timer += GloomEngine::GetInstance()->deltaTime;
    if (timer >= pattern->sounds[sampleIndex]->delay) {
        timer = 0.0f;
        sampleSources[sampleIndex]->ForcePlaySound();
        sampleIndex++;
        if (sampleIndex >= pattern->sounds.size())
            sampleIndex = 0;
    }

    if (!triggerActive) return;

    // Show first dialogue
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_E) && !dialogueActive && !sessionStarter) {
        dialogue->enabled = true;
        dialogueActive = true;
        return;
    }

    // Show choose menu
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER) && dialogueActive && !chooseMenuActive && !rejectDialogueActive && !acceptDialogueActive) {
        chooseMenuActive = true;
        button1->isActive = false;
        button2->isActive = true;
        chooseMenu->EnableSelfAndChildren();
        DialogueManager::GetInstance()->NotifyMenuIsActive();
        return;
    }

    // Choose menu
    if (chooseMenuActive) {
        if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_A) || HIDManager::GetInstance()->IsKeyDown(Key::KEY_ARROW_LEFT)) {
            button1->isActive = true;
            button2->isActive = false;
        }
        if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_D) || HIDManager::GetInstance()->IsKeyDown(Key::KEY_ARROW_RIGHT)) {
            button1->isActive = false;
            button2->isActive = true;
        }
        if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER)) {
            if (button1->isActive) {
                chooseMenuActive = false;
                chooseMenu->DisableSelfAndChildren();
                dialogue->texts[1].text2 = "Walcz!";
                DialogueManager::GetInstance()->NotifyMenuIsNotActive();
                dialogue->image->enabled = false;
                acceptDialogueActive = true;
                return;
            } else {
                chooseMenuActive = false;
                chooseMenu->DisableSelfAndChildren();
                dialogue->texts[1].text2 = "Nie to nie.";
                DialogueManager::GetInstance()->NotifyMenuIsNotActive();
                dialogue->image->enabled = false;
                rejectDialogueActive = true;
                return;
            }
        }
    }

    // Hide reject dialogue
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER) && rejectDialogueActive) {
        dialogue->enabled = false;
        dialogue->HideDialogue();
        dialogueActive = false;
        rejectDialogueActive = false;
        dialogue->image->enabled = true;
        return;
    }

    // Hide accept dialogue
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER) && acceptDialogueActive) {
        dialogue->enabled = false;
        dialogue->HideDialogue();
        dialogueActive = false;
        acceptDialogueActive = false;
        dialogue->image->enabled = false;
        CreateSessionStarter();
        return;
    }

    // Session starter
    if (sessionStarter) {
        if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ARROW_LEFT))
            sessionStarter->ChangeActiveButton(glm::vec2(-1, 0));
        if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ARROW_RIGHT))
            sessionStarter->ChangeActiveButton(glm::vec2(1, 0));
        if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER)) {
            sessionStarter->OnClick();
            GameObject::Destroy(sessionStarter->GetParent());
            sessionStarter.reset();
        }
    }

    // End of time
    if (wellPlayedPatternCount >= 10) {
        defeated = true;
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->OnPlayerLoseDuel();
        winDialogue->ShowDialogue();
    }

    Component::Update();
}

void Opponent::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
    ui->EnableSelfAndChildren();
    for (const auto & i : belt) {
        i->enabled = false;
    }
    Component::OnTriggerEnter(gameObject);
}

void Opponent::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = false;
    ui->DisableSelfAndChildren();
    Component::OnTriggerExit(gameObject);
}

void Opponent::UpdateSatisfaction(float satisfaction1) {
    if (triggerActive) {
        for (const auto & i : belt) {
            i->enabled = false;
        }
        if (satisfaction1 >= satisfaction) wellPlayedPatternCount++;
        if (satisfactionAverage == 0.0f) {
            satisfactionAverage = satisfaction1;
            for (int i = 0; i < (int)satisfactionAverage / 5; i++) {
                belt[i]->enabled = true;
            }
            return;
        }
        satisfactionAverage += satisfaction1;
        satisfactionAverage /= 2;
        for (int i = 0; i < (int)satisfactionAverage / 5; i++) {
            belt[i]->enabled = true;
        }
    }
}

void Opponent::CreateSessionStarter() {
    playerManager->inputEnabled = false;
    GloomEngine::GetInstance()->timeScale = 0;
    sessionStarter = GameObject::Instantiate("OpponentSessionStarter", parent)->AddComponent<OpponentSessionStarter>();
    sessionStarter->Setup(playerManager->GetParent()->GetComponent<PlayerEquipment>()->instruments);
}

void Opponent::CreateMusicSession(InstrumentName instrumentName) {
    // TODO create music session
//    musicSession = GameObject::Instantiate("OpponentMusicSession", parent)->AddComponent<OpponentMusicSession>();
//    musicSession->Setup(equipment->GetInstrumentWithName(instrumentName));
//    AIManager::GetInstance()->NotifyPlayerStartsPlaying(instrumentName, equipment->GetInstrumentWithName(instrumentName)->genre);
}

void Opponent::OnCreate() {
    OpponentManager::GetInstance()->opponents.insert({id, std::dynamic_pointer_cast<Opponent>(shared_from_this())});
    Component::OnCreate();
}

void Opponent::OnDestroy() {
    OpponentManager::GetInstance()->opponents.erase(id);
    ui.reset();
    instrument.reset();
    pattern.reset();
    sampleSources.clear();
    playerManager.reset();
    button1.reset();
    button2.reset();
    sessionStarter.reset();
    chooseMenu.reset();
    belt.clear();
    dialogue.reset();
    winDialogue.reset();
    lossDialogue.reset();
    Component::OnDestroy();
}