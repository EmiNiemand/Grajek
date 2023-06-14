//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_PLAYERMANAGER_H
#define GLOOMENGINE_PLAYERMANAGER_H


#include "Components/Component.h"
#include "Interfaces/IDataPersistance.h"
#include "Components/Scripts/MusicPattern.h"
#include "glm/vec2.hpp"
#include <set>

class PlayerMovement;
class Rigidbody;
class PlayerUI;
class PlayerEquipment;
class Instrument;
class MusicSession;
class SessionStarter;
class Opponent;
class PauseMenu;
class OptionsMenu;
class ShopMenu;
class SavePointMenu;
class Menu;
class Animator;


class PlayerManager : public Component, public IDataPersistance {
private:
	// Music session
    std::shared_ptr<MusicSession> session;
    std::shared_ptr<SessionStarter> sessionStarter;
    std::shared_ptr<GameObject> sessionStarterUI;
    std::shared_ptr<Opponent> sessionOpponent;
    bool sessionMetronomeSound, sessionMetronomeVisuals, sessionBackingTrack;
	// Player scripts
	std::shared_ptr<PlayerUI> playerUI;
    std::shared_ptr<PlayerEquipment> equipment;
	std::shared_ptr<PlayerMovement> movement;
	// UI
    std::shared_ptr<PauseMenu> pauseMenu;
    std::shared_ptr<OptionsMenu> optionsMenu;
    std::shared_ptr<ShopMenu> shopMenu;
    std::shared_ptr<SavePointMenu> savePointMenu;
	// Player components
    std::shared_ptr<Animator> animator;
	std::shared_ptr<Rigidbody> rb;
	//PlayerCollider collider;
	//PlayerCamera camera;
	// Local variables
    std::shared_ptr<AudioSource> stepSound;
    glm::vec2 moveInput;
	float previousVelocity;

public:
    std::shared_ptr<Menu> activeMenu;
    bool inputEnabled;

public:
    PlayerManager(const std::shared_ptr<GameObject> &parent, int id);

    // Component methods
    void Awake() override;
    void Start() override;
    void Update() override;
    void OnDestroy() override;

    // Equipment methods
    bool BuyInstrument(int price, const std::shared_ptr<Instrument>& instrument);
    int GetCash();

    // Session methods
    void StartSessionWithOpponent(const std::shared_ptr<Opponent>& opponent);
    void EndSessionWithOpponent(bool wonSession, float moneyBet);
    // Argument pat is null when player failed playing pattern
    void PlayedPattern(const std::shared_ptr<MusicPattern>& pat, float accuracy);
    void CreateMusicSession(InstrumentName instrument);

    // UI methods
    void ToggleOptionsMenu();

    // Equipment methods
    std::set<InstrumentName> GetInstruments();

    // IDataPersistance methods
    void LoadData(std::shared_ptr<GameData> data) override;
    void SaveData(std::shared_ptr<GameData> &data) override;

    void OnSoundStop(int index);

private:
    void PollInput();

    // Session methods
    void OnSessionToggle();
    void OnSoundPlay(int index);
    void OnCheatSheetToggle();
    void OnInstrumentControlToggle();
    void OnMetronomeSoundToggle();
    void OnMetronomeVisualsToggle();
    void OnBackingTrackToggle();

	// Animation methods
	void UpdateAnimations();

    // Movement methods
    void OnMove(glm::vec2 moveVector);
    void OnInteract();

    // UI methods
    void OnUIMove(glm::vec2 moveVector);
	void OnMenuToggle();
	void OnApply();
};


#endif //GLOOMENGINE_PLAYERMANAGER_H
