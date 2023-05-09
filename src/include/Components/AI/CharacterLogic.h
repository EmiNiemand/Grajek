//
// Created by Adrian on 01.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERLOGIC_H
#define GLOOMENGINE_CHARACTERLOGIC_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include "Components/AI/CharacterStates.h"
#include "Components/Scripts/MusicPattern.h"

#include <vector>
#include <unordered_map>

class GameObject;
class CharacterMovement;
class Animator;

class CharacterLogic : public Component {
    std::shared_ptr<CharacterMovement> characterMovement;
    std::shared_ptr<Animator> characterAnimation;
    float minSatisfaction = 0;
    float currentSatisfaction = 0;
    AI_LOGICSTATE currentState = WalkingOnPath;

    InstrumentName playerInstrumentName;
    MusicGenre playerGenre;
    std::shared_ptr<MusicPattern> playerPattern;

public:
    std::vector<InstrumentName> favInstrumentsNames;
    std::vector<MusicGenre> favGenres;
    std::vector<std::shared_ptr<MusicPattern>> favPatterns;

    CharacterLogic(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterLogic() override;

    void Update() override;
    void OnCreate() override;
    void OnDestroy() override;
    void Free();

    void SetPathToPlayer();
    void ReturnToPreviousPath();
    void SetPlayerInstrumentAndGenre(const InstrumentName &ins, const MusicGenre &gen);
    void SetPlayerPattern(const std::shared_ptr<MusicPattern> &pat);
    void SetPlayerPlayingStatus(bool state);
    void CalculateSatisfaction();
    const float GetCurrentSatisfaction() const;

};

#endif //GLOOMENGINE_CHARACTERLOGIC_H
