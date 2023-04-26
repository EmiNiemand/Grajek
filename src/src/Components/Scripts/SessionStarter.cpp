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
        // 100 -- buttonWidth / 2
        // 200 -- buttonWidth
        std::shared_ptr<GameObject> button = Menu::AddButton(std::to_string((int)instrument->name), 960 - instruments.size() * 100 + i * 200 + 100 * ((i + 1) - instruments.size()) + 50 * i + 25, 450, "UI/buttonInactive.png", "UI/buttonActive.png", instrument->NameToString(), 32);
        buttons.push_back(button->GetComponent<Button>());
        i++;
    }
    if (buttons.size() > 1) {
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

void SessionStarter::ChangeActiveButton(glm::vec2 moveVector) {
    if (moveVector.x == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->nextButton;
        activeButton->isActive = true;
    }
    if (moveVector.x == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->previousButton;
        activeButton->isActive = true;
    }
}

void SessionStarter::OnClick() {
    auto playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    playerManager->CreateMusicSession((InstrumentName)std::stoi(activeButton->GetParent()->GetName()));
}

void SessionStarter::Stop() {
    parent->parent->RemoveAllChildren();
}