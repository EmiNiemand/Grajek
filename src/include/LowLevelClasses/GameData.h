//
// Created by szymo on 24/03/2023.
//

#ifndef GLOOMENGINE_GAMEDATA_H
#define GLOOMENGINE_GAMEDATA_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

class GameData {
public:
    int money;
    int reputation;
    glm::vec3 playerPosition;
    //TODO: add instrument and define its start value in the constructor

public:
    GameData();
};


#endif //GLOOMENGINE_GAMEDATA_H
