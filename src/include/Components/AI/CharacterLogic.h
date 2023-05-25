//
// Created by Adrian on 01.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERLOGIC_H
#define GLOOMENGINE_CHARACTERLOGIC_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include "Components/Scripts/MusicPattern.h"
#include "Components/AI/CharacterStates.h"
#include <vector>

class GameObject;
class CharacterAnimations;
class CharacterMovement;
class Indicator;

class CharacterLogic : public Component {
    AI_LOGICSTATE logicState = None;
    std::shared_ptr<CharacterAnimations> characterAnimations = nullptr;
    std::shared_ptr<CharacterMovement> characterMovement = nullptr;
    std::shared_ptr<Indicator> characterIndicator = nullptr;
    float minSatisfaction = 0;
    float currentSatisfaction = 0;

    InstrumentName playerInstrumentName {};
    MusicGenre playerGenre {};
    std::shared_ptr<MusicPattern> playerPattern = nullptr;

public:
    std::vector<InstrumentName> favInstrumentsNames {};
    std::vector<MusicGenre> favGenres {};
    std::vector<std::shared_ptr<MusicPattern>> favPatterns {};

    CharacterLogic(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterLogic() override;

    void Start() override;
    void Update() override;
    void AIUpdate() override;
    void OnCreate() override;
    void OnDestroy() override;

    void SetPlayerInstrumentAndGenre(const InstrumentName &ins, const MusicGenre &gen);
    void SetPlayerPattern(const std::shared_ptr<MusicPattern> &pat);
    void SetPlayerPlayingStatus(bool state);
    void CalculateSatisfaction();
    const float GetCurrentSatisfaction() const;

};

#endif //GLOOMENGINE_CHARACTERLOGIC_H
