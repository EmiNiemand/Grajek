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

constexpr float ANNOYED_SATISFACTION_REDUCER = 0.20f;
constexpr float NORMAL_SATISFACTION_REDUCER = 0.03f;
constexpr float PREVIOUS_SESSION_TIMEOUT = 10.0f;

class GameObject;
class CharacterAnimations;
class CharacterMovement;

class CharacterLogic : public Component {
    AI_LOGIC_STATE logicState = Wandering;
    float timeSinceSession = 0.0f;
    float timeSinceOnFrustum = 0.0f;
    // Animations
    std::shared_ptr<CharacterAnimations> characterAnimations = nullptr;
    std::string modelPath {};
    // Movement and indicators
    std::shared_ptr<CharacterMovement> characterMovement = nullptr;
    // Player values and modifiers
    bool isPlayerPlaying = false;
    std::shared_ptr<Transform> playerTransform = nullptr;
    glm::vec3 playerPosition {};
    InstrumentName playerInstrumentName {};
    MusicGenre playerGenre {};
    InstrumentName previousPlayerInstrumentName {};
    MusicGenre previousPlayerGenre {};
    float values = 0.0f;
    float repeatingModifier = 0.0f;
    // Enemy instruments
    InstrumentName opponentInstrumentName {};
    MusicGenre opponentGenre {};
    // Satisfaction
    float lowerSatisfactionLimit = 0.0f;
    float middleSatisfactionLimit = 0.0f;
    float upperSatisfactionLimit = 0.0f;
    float playerSatisfaction = 0.0f;
    float opponentSatisfaction = 0.0f;

    void CalculateBaseSatisfaction();

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
    void SetPlayerPattern(const std::shared_ptr<MusicPattern> &pattern);
    void SetPlayerPlayingStatus(const bool& isPlayerPlaying);
    const float GetPlayerSatisfaction() const;
    void SetOpponentInstrumentAndGenre(const InstrumentName &instrument, const MusicGenre &genre);
    void SetOpponentPattern(const std::shared_ptr<MusicPattern> &pattern);
    void SetOpponentPlayingStatus(const bool& isOpponentPlaying);
    const float GetOpponentSatisfaction() const;
    const AI_LOGIC_STATE GetLogicState() const;

};

#endif //GLOOMENGINE_CHARACTERLOGIC_H
