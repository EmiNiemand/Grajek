//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_PLAYERMANAGER_H
#define GLOOMENGINE_PLAYERMANAGER_H


#include "PlayerMovement.h"
#include "PlayerEquipment.h"
#include "glm/vec2.hpp"

class PlayerManager : public Component {
private:
    std::shared_ptr<PlayerMovement> movement;
//    PlayerCamera camera;
//    PlayerUI ui;
//    PlayerCollider collider;
    std::shared_ptr<PlayerEquipment> equipment;

public:
    bool inputEnabled;
	bool uiActive;
    glm::vec2 moveVector;

private:
    void Start() override;
    void Update() override;

    void PollInput();

    void OnMove(glm::vec2 moveVector);
    void OnInteract();
	void OnSaveLoad(bool save);
	void OnMenuToggle();
	void OnApply();

public:
    PlayerManager(const std::shared_ptr<GameObject> &parent, int id);
};


#endif //GLOOMENGINE_PLAYERMANAGER_H
