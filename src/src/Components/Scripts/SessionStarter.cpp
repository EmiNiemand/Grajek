#include "Components/Scripts/SessionStarter.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/PlayerManager.h"

SessionStarter::SessionStarter(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

SessionStarter::~SessionStarter() {}

void SessionStarter::Setup(const std::set<std::shared_ptr<Instrument>>& instruments) {
    int i = 0;
    for (const auto& instrument : instruments)
    {
        std::shared_ptr<GameObject> button = Menu::AddButton(std::to_string((int)instrument->name), i * 250 + 50, 450, "UI/buttonInactive.png", "UI/buttonActive.png", instrument->NameToString(), 32);
        buttons.push_back(button->GetComponent<Button>());
        i++;
    }
    if (buttons.size() > 1) {
        spdlog::info("No większe");
        i = 0;
        for (const auto &button: buttons) {
            if (i != 0) button->previousButton = buttons[i - 1];
            if (i != buttons.size()-1) button->nextButton = buttons[i + 1];
            i++;
        }
        buttons[0]->previousButton = buttons[buttons.size() - 1];
        buttons[buttons.size() - 1]->nextButton = buttons[0];
    }
    activeButton = buttons[0];
    buttons[0]->isActive = true;
}

void SessionStarter::OnClick() {
    spdlog::info("AAAAAAAAAAAAAA");
    auto playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    playerManager->CreateMusicSession((InstrumentName)std::stoi(activeButton->GetParent()->GetName()));

//    if (GloomEngine::GetInstance()->FindGameObjectWithName("Instrument")->GetComponent<Button>()->isActive) {
//        playerManager->CreateMusicSession(player->GetComponent<PlayerEquipment>()->instruments.begin());
//    } else if (GloomEngine::GetInstance()->FindGameObjectWithName("Instrument1")->GetComponent<Button>()->isActive) {
////        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->CreateMusicSession(GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerEquipment>()->instruments.);
//    }
}

void SessionStarter::Stop() {
    parent->parent->RemoveAllChildren();
}
