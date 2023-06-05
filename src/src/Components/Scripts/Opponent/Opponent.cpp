#include "GloomEngine.h"
#include "Components/Scripts/Opponent/Opponent.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/HIDManager.h"
#include "Components/UI/Button.h"
#include "EngineManagers/DialogueManager.h"
#include "Components/Scripts/Opponent/OpponentSessionStarter.h"
#include "Components/Scripts/Player/PlayerEquipment.h"
#include "Components/Scripts/Opponent/OpponentMusicSession.h"
#include "EngineManagers/AIManager.h"
#include "Components/Scripts/SessionUI/SessionUI.h"
#include "Components/Renderers/Camera.h"

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
        sample->SetPositionOffset(parent->transform->GetLocalPosition());
    }
    satisfaction = satisfaction1;

    // Setup UI
    ui = GameObject::Instantiate("OpponentUI", parent);
    GameObject::Instantiate("OpponentFrame", ui)->AddComponent<Image>()->LoadTexture(460, 880, "UI/RamkaPrzeciwnik.png");
    belt = GameObject::Instantiate("OpponentSatisfactionDifference", ui)->AddComponent<Image>();
    belt->LoadTexture(960, 880, "UI/Satisfaction.png");
    belt->SetScale(0.0f);
    timeCounter = GameObject::Instantiate("OpponentTimeCounter", ui)->AddComponent<Image>();
    timeCounter->LoadTexture(460, 865, "UI/TimeCounter.png");
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
    // Play pattern
    timer += GloomEngine::GetInstance()->deltaTime;
    if (timer >= pattern->sounds[sampleIndex]->delay) {
        timer = 0.0f;
        sampleSources[sampleIndex]->ForcePlaySound();
        sampleIndex++;
        if (sampleIndex >= pattern->sounds.size())
            sampleIndex = 0;
    }

    if (!dialogue->triggerActive) return;

    auto hid = HIDManager::GetInstance();

    if (hid->IsKeyDown(Key::KEY_ENTER) && winDialogue->active && winDialogue->dialogueIndex)
        dialogue->image->enabled = true;

    // Show first dialogue
    if (hid->IsKeyDown(Key::KEY_E) && !dialogueActive && !sessionStarter && !musicSession && !lossDialogue->active && !winDialogue->active) {
        dialogue->enabled = true;
        dialogueActive = true;
        return;
    }

    if (defeated) return;

    // Show choose menu
    if (hid->IsKeyDown(Key::KEY_ENTER) && dialogueActive && !chooseMenuActive && !rejectDialogueActive && !acceptDialogueActive) {
        chooseMenuActive = true;
        button1->isActive = false;
        button2->isActive = true;
        chooseMenu->EnableSelfAndChildren();
        DialogueManager::GetInstance()->NotifyMenuIsActive();
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
            DialogueManager::GetInstance()->NotifyMenuIsNotActive();
            dialogue->image->enabled = false;
            if (button1->isActive) {
                dialogue->texts[1].text2 = "Walcz!";
                acceptDialogueActive = true;
                return;
            } else {
                dialogue->image->enabled = false;
                dialogue->texts[1].text2 = "Nie to nie.";
                rejectDialogueActive = true;
                return;
            }
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
        CreateSessionStarter();
        DialogueManager::GetInstance()->NotifyMenuIsActive();
        return;
    }

    // Session starter
    if (sessionStarter) {
        if (hid->IsKeyDown(Key::KEY_ARROW_LEFT))
            sessionStarter->ChangeActiveButton(glm::vec2(-1, 0));
        if (hid->IsKeyDown(Key::KEY_ARROW_RIGHT))
            sessionStarter->ChangeActiveButton(glm::vec2(1, 0));
        if (hid->IsKeyDown(Key::KEY_ENTER)) {
            sessionStarter->OnClick();
            GameObject::Destroy(sessionStarter->GetParent());
            sessionStarter.reset();
            return;
        }
    }

    // Music session
    if (musicSession) {
        time += GloomEngine::GetInstance()->deltaTime;
        timeCounter->SetScale(glm::vec2(1 - time / battleTime, 1));
        if (hid->IsKeyDown(Key::KEY_LEFT_SHIFT))
            musicSession->ToggleInstrumentControl();
        if (hid->IsKeyDown(Key::KEY_TAB))
            musicSession->ToggleCheatSheet();
        for (auto key: PlaySound) {
            if (hid->IsKeyDown(key.first)) musicSession->PlaySample(key.second);
            if (hid->IsKeyUp(key.first)) musicSession->StopSample(key.second);
        }
        if (time >= battleTime || satisfactionDifference >= 100 || satisfactionDifference <= -100) {
            if (satisfactionDifference <= -100 || (time >= battleTime && satisfactionDifference <= 0)) {
                lossDialogue->ShowDialogue();
                time = 0.0f;
                satisfactionDifference = 0.0f;
            }
            if (satisfactionDifference >= 100 || (time >= battleTime && satisfactionDifference > 0)) {
                defeated = true;
                winDialogue->ShowDialogue();
                dialogue->texts.pop_back();
                dialogue->texts[0].text2 = "To byla dobra bitwa. Pokonaj kolejnego przeciwnika.";
                // TODO add sound when player beat boss
            }
            ui->DisableSelfAndChildren();
            GameObject::Destroy(musicSession->GetParent());
            musicSession.reset();
            GameObject::Destroy(sessionUI->GetParent());
            sessionUI.reset();
            Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(1.0f);
            AIManager::GetInstance()->NotifyPlayerStopsPlaying();
            DialogueManager::GetInstance()->NotifyMenuIsNotActive();
            dialogue->image->enabled = false;
        }
        return;
    }

    if (hid->IsKeyDown(Key::KEY_ENTER) && lossDialogue->active)
        dialogue->image->enabled = true;

    Component::Update();
}

void Opponent::UpdateSatisfaction(float satisfaction1) {
    float s = satisfaction1 - satisfaction;
    satisfactionDifference += s;
    belt->SetScale(glm::vec2(satisfactionDifference / 100, 1.0f));
}

void Opponent::CreateSessionStarter() {
    playerManager->inputEnabled = false;
    GloomEngine::GetInstance()->timeScale = 0;
    sessionStarter = GameObject::Instantiate("OpponentSessionStarter", parent)->AddComponent<OpponentSessionStarter>();
    sessionStarter->Setup(playerManager->GetParent()->GetComponent<PlayerEquipment>()->instruments);
}

void Opponent::CreateMusicSession(InstrumentName instrumentName) {
    auto equipment = playerManager->GetParent()->GetComponent<PlayerEquipment>();
    musicSession = GameObject::Instantiate("OpponentMusicSession", parent)->AddComponent<OpponentMusicSession>();
    musicSession->Setup(equipment->GetInstrumentWithName(instrumentName));
    AIManager::GetInstance()->NotifyPlayerStartsPlaying(instrumentName, equipment->GetInstrumentWithName(instrumentName)->genre);
    belt->SetScale(0);
    timeCounter->SetScale(1);
    ui->EnableSelfAndChildren();
}

void Opponent::OnDestroy() {
    ui.reset();
    instrument.reset();
    pattern.reset();
    sampleSources.clear();
    playerManager.reset();
    button1.reset();
    button2.reset();
    sessionStarter.reset();
    chooseMenu.reset();
    belt.reset();
    dialogue.reset();
    winDialogue.reset();
    lossDialogue.reset();
    musicSession.reset();
    sessionUI.reset();
    timeCounter.reset();
    Component::OnDestroy();
}