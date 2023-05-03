//
// Created by Adrian on 01.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERLOGIC_H
#define GLOOMENGINE_CHARACTERLOGIC_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include "Components/Scripts/MusicPattern.h"
#include <vector>
#include <unordered_map>

class GameObject;
class CharacterMovement;

class CharacterLogic : public Component {
    std::shared_ptr<Transform> player;
    std::shared_ptr<CharacterMovement> characterMovement;
    float minSatisfaction = 0;
    float currentSatisfaction = 0;
    bool playerIsPlaying = false;
    bool onPathToPlayer = false;
    float playerSkill;

    InstrumentName playerInstrumentName;
    MusicGenre playerGenre;
// Music.Helpers.Pattern playerPattern;

public:
    std::vector<InstrumentName> favInstrumentsNames;
    std::vector<MusicGenre> favGenres;
//    [SerializeField] public List<Music.Helpers.Pattern> favPatterns;

    CharacterLogic(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterLogic() override;

    void Start() override;
    void AIUpdate() override;
    void OnCreate() override;
    void OnDestroy() override;
    void Free();

    void CalculateSatisfaction();
    void SetPathToPlayer() const;
    void ReturnToPreviousPath() const;
    void SetPlayerInstrumentAndGenre(const InstrumentName &ins, const MusicGenre &gen);
    void SetPlayerStatus();
    float GetCurrentSatisfaction() const;

};

#endif //GLOOMENGINE_CHARACTERLOGIC_H
