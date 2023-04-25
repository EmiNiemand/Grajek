//
// Created by szymo on 24/03/2023.
//

#ifndef GLOOMENGINE_GAMEDATA_H
#define GLOOMENGINE_GAMEDATA_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Components/Scripts/MusicPattern.h"
#include <set>

class GameData {
public:
    int money;
    int reputation;
    glm::vec3 playerPosition;
    std::set<InstrumentName> instruments;

public:
    GameData();
};


#endif //GLOOMENGINE_GAMEDATA_H
