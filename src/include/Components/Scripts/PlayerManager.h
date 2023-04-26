//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_PLAYERMANAGER_H
#define GLOOMENGINE_PLAYERMANAGER_H


#include "Components/Component.h"
#include "Interfaces/IDataPersistance.h"
#include "MusicPattern.h"
#include "glm/vec2.hpp"
#include <set>

class PlayerMovement;
class PlayerUI;
class PlayerEquipment;
class Instrument;
class MusicSession;
class SessionStarter;
class PauseMenu;
class OptionsMenu;
class ShopMenu;
class Menu;


class PlayerManager : public Component, public IDataPersistance {
private:
    std::shared_ptr<PlayerMovement> movement;
//    PlayerCamera camera;
    std::shared_ptr<PlayerUI> playerUI;
    std::shared_ptr<MusicSession> session;
    std::shared_ptr<SessionStarter> sessionStarter;
    std::shared_ptr<GameObject> sessionStarterUI;
//    PlayerCollider collider;
    std::shared_ptr<PlayerEquipment> equipment;
    std::shared_ptr<PauseMenu> pauseMenu;
    std::shared_ptr<OptionsMenu> optionsMenu;
    std::shared_ptr<ShopMenu> shopMenu;
    std::shared_ptr<Menu> activeMenu;
    glm::vec2 moveInput;
    bool cheatSheetActive = false;

public:
    bool inputEnabled;

//Equipment methods
bool BuyInstrument(int price, const std::shared_ptr<Instrument>& instrument);

private:
    void Start() override;
    void Awake() override;
    void Update() override;

    void PollInput();

    //Session methods
    void OnSessionToggle();
    void OnSoundPlay(int index);
    void OnCheatSheetToggle();

    //Movement methods
    void OnMove(glm::vec2 moveVector);
    void OnInteract();
    //UI methods
    void OnUIMove(glm::vec2 moveVector);
	void OnMenuToggle();
	void OnApply();

public:
    PlayerManager(const std::shared_ptr<GameObject> &parent, int id);

    // Session methods
    // Argument pat is null when player failed playing pattern
    void PlayedPattern(const std::shared_ptr<MusicPattern>& pat);
    void CreateMusicSession(InstrumentName instrument);

    // UI methods
    void ToggleOptionsMenu();

    // Equipment methods
    std::set<InstrumentName> GetInstruments();

    // IDataPersistance methods
    void LoadData(std::shared_ptr<GameData> data) override;
    void SaveData(std::shared_ptr<GameData> &data) override;

    void OnSoundStop(int index);
};


#endif //GLOOMENGINE_PLAYERMANAGER_H
