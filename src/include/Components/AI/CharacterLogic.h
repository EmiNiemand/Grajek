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

constexpr float SATISFACTION_REDUCER = 2.0f;
constexpr float PREVIOUS_SESSION_TIMEOUT = 5.0f;
constexpr float DESPAWN_TIMEOUT = 7.5f;

class GameObject;
class CharacterAnimations;
class CharacterMovement;
class Indicator;

class CharacterLogic : public Component {
    AI_LOGICSTATE logicState = Wandering;
    float timeSinceSession = 0.0f;
    float timeSinceOnFrustum = 0.0f;
    // Animations
    std::shared_ptr<CharacterAnimations> characterAnimations = nullptr;
    std::string modelPath {};
    // Movement and indicators
    std::shared_ptr<CharacterMovement> characterMovement = nullptr;
    std::shared_ptr<Indicator> characterIndicator = nullptr;
    // Player values and modifiers
    InstrumentName playerInstrumentName {};
    MusicGenre playerGenre {};
    std::shared_ptr<MusicPattern> playerPattern = nullptr;
    InstrumentName previousPlayerInstrumentName {};
    MusicGenre previousPlayerGenre {};
    float values = 0.0f;
    float repeatingModifier = 0.0f;
    // Enemy instruments
    InstrumentName enemyInstrumentName {};
    MusicGenre enemyGenre {};
    std::shared_ptr<MusicPattern> enemyPattern = nullptr;
    // Satisfaction
    float minSatisfaction = 0.0f;
    float lowerSatisfactionLimit = 0.0f;
    float middleSatisfactionLimit = 0.0f;
    float upperSatisfactionLimit = 0.0f;
    float playerSatisfaction = 0.0f;
    float enemySatisfaction = 0.0f;

    void CalculateSatisfaction();

public:
    // Favorite instruments and patterns
    std::vector<InstrumentName> favInstrumentsNames {};
    std::vector<MusicGenre> favGenres {};
    // int - pattern.id, float - repeating modifier
    std::vector<std::pair<int, float>> favPatterns {};

    CharacterLogic(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterLogic() override;

    void Start() override;
    void Update() override;
    void AIUpdate() override;
    void OnCreate() override;
    void OnDestroy() override;

    void SetAnimationModelToLoad(const std::string& model);
    void SetPlayerInstrumentAndGenre(const InstrumentName &instrument, const MusicGenre &genre);
    void SetPlayerPattern(const std::shared_ptr<MusicPattern> &pat);
    void SetPlayerPlayingStatus(const bool& isPlayerPlaying);
    const float GetPlayerSatisfaction() const;
    void SetEnemyInstrumentAndGenre(const InstrumentName &instrument, const MusicGenre &genre);
    void SetEnemyPattern(const std::shared_ptr<MusicPattern> &pat);
    void SetEnemyPlayingStatus(const bool& isEnemyPlaying);
    const float GetEnemySatisfaction() const;

};

#endif //GLOOMENGINE_CHARACTERLOGIC_H
