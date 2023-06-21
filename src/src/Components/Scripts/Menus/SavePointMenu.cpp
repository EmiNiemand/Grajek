#include "Components/Scripts/Menus/SavePointMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/DataPersistanceManager.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"
#include "EngineManagers/SavePointManager.h"
#include "LowLevelClasses/GameData.h"
#include "EngineManagers/SceneManager.h"
#include <filesystem>
#include <ctime>

SavePointMenu::SavePointMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

SavePointMenu::~SavePointMenu() = default;

void SavePointMenu::OnDestroy() {
    buttonImage.reset();
    Menu::OnDestroy();
}

bool SavePointMenu::ShowMenu() {
    if (!triggerActive) return false;
    if(!Menu::ShowMenu()) return false;

    buttonImage->enabled = false;
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("Save1")->GetComponent<Button>();
    activeButton->isActive = true;
    return true;
}

void SavePointMenu::OnClick() {
    if (!activeButton) return;
    buttonImage->enabled = true;
    auto animator = GameObject::Instantiate("SavePointMenuAnimator", SavePointManager::GetInstance()->activeSavePoint->GetParent());
    auto image = GameObject::Instantiate("SavePointMenuImage", animator)->AddComponent<Image>();
    image->LoadTexture(0, 0, "UI/ZapisGry.png");
    image->SetPosition(960 - image->GetWidth()/2, 700);
    animator->AddComponent<UIAnimator>()->Setup(image, {
            {AnimatedProperty::Alpha, glm::vec3(1.0f), 0.2f},
            {AnimatedProperty::Alpha, glm::vec3(0.0f), 2.0f}
    }, AnimationBehaviour::OneTime);

    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "ProjectConfig";
    path /= "Saves";

    saveDate = "";
    time_t now = time(0);
    std::string date = ctime(&now);
    std::size_t day = date.find_first_of("123456789");
    if (date[day + 1] == ' ') saveDate += '0';
    saveDate += date[day];
    if (date[day + 1] != ' ') saveDate += date[day + 1];
    if (date.find("Jan") != std::string::npos) saveDate += ".01.";
    if (date.find("Feb") != std::string::npos) saveDate += ".02.";
    if (date.find("Mar") != std::string::npos) saveDate += ".03.";
    if (date.find("Apr") != std::string::npos) saveDate += ".04.";
    if (date.find("May") != std::string::npos) saveDate += ".05.";
    if (date.find("Jun") != std::string::npos) saveDate += ".06.";
    if (date.find("Jul") != std::string::npos) saveDate += ".07.";
    if (date.find("Aug") != std::string::npos) saveDate += ".08.";
    if (date.find("Sep") != std::string::npos) saveDate += ".09.";
    if (date.find("Oct") != std::string::npos) saveDate += ".10.";
    if (date.find("Nov") != std::string::npos) saveDate += ".11.";
    if (date.find("Dec") != std::string::npos) saveDate += ".12.";
    saveDate += date.substr(date.size() - 5, 4);
    if (date[day + 1] == ' ') saveDate += date.substr(day + 1, 6);
    else saveDate += date.substr(day + 2, 6);

    char save = activeButton->text.back();
    if (save == '0') saveDates.back()->text = saveDate;
    else saveDates[(int)save - 49]->text = saveDate;

    DataPersistanceManager::GetInstance()->SaveGame(path.string(), activeButton->text);
    HideMenu();
    GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->activeMenu.reset();
    Menu::OnClick();
}

void SavePointMenu::HideMenu() {
    Menu::HideMenu();
    if (triggerActive) buttonImage->enabled = true;
}

void SavePointMenu::SaveData(std::shared_ptr<GameData> &data) {
    data->saveDate = saveDate;
}