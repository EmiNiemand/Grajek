#include "Components/Scripts/SessionStarter.h"
#include "Components/Scripts/Instrument.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"

SessionStarter::SessionStarter(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

SessionStarter::~SessionStarter() {}

void SessionStarter::Setup(const std::set<std::shared_ptr<Instrument>>& instruments) {
    int buttonWidth = 256;
    int buttonHeight = 300;
    int margin = 100;

    int buttonOffset = buttonWidth + margin;
                    // We start from the middle of a screen...
    int xPosBegin = 960 -
            // Offset pivot to button's middle point...
            buttonWidth/2.0f -
            // Try to distribute this nicely around the screen's middle...
            (instruments.size()/2) * buttonOffset -
            // And - if there's uneven amount of instruments - move
            // all buttons so that middle instruments is in the middle
            // of the screen
            (buttonOffset/2.0f) * ((instruments.size()+1) % 2);

    // Sort instrument names
    std::map<InstrumentName, std::string> instrumentNames;

    for(const auto& instrument : instruments) {
        instrumentNames.insert({instrument->name, instrument->NameToString()});
    }

    for (const auto& instrument : instrumentNames)
    {
        auto button = Menu::AddButton(
                std::to_string((int)instrument.first),
                xPosBegin + buttonOffset * buttons.size(),
                540 - buttonHeight/2.0f,
                "UI/Icons/small/icon"+instrument.second+"Inactive.png",
                "UI/Icons/small/icon"+instrument.second+".png");
        buttons.push_back(button);
    }
    if (buttons.size() > 1) {
        int i = 0;
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
    AddImage("DarkBackground", 0, 0, "UI/backgroundOpacity90.png");
}

void SessionStarter::ChangeActiveButton(glm::vec2 moveVector) {
    if (!activeButton->previousButton) return;
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

void SessionStarter::OnDestroy() {
    buttons.clear();
    Menu::OnDestroy();
}
