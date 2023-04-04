//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_PLAYERMANAGER_H
#define GLOOMENGINE_PLAYERMANAGER_H


#include "PlayerMovement.h"
#include "PlayerEquipment.h"
#include "PlayerUI.h"
#include "glm/vec2.hpp"

class PlayerManager : public Component {
private:
    std::shared_ptr<PlayerMovement> movement;
//    PlayerCamera camera;
    std::shared_ptr<PlayerUI> ui;
//    PlayerCollider collider;
    std::shared_ptr<PlayerEquipment> equipment;
    glm::vec2 moveInput;

public:
    bool inputEnabled;
	bool uiActive;

private:
    void Start() override;
    void Update() override;

    void PollInput();

    //Equipment methods
    bool BuyInstrument(int price, const std::shared_ptr<Instrument>& instrument);
    //Movement methods
    void OnMove(glm::vec2 moveVector);
    void OnInteract();
    //Data methods
	void OnSaveLoad(bool save);
    //UI methods
    void OnUIMove(glm::vec2 moveVector);
	void OnMenuToggle();
	void OnApply();

public:
    PlayerManager(const std::shared_ptr<GameObject> &parent, int id);
};


#endif //GLOOMENGINE_PLAYERMANAGER_H
