//
// Created by Adrian on 04.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERSTATES_H
#define GLOOMENGINE_CHARACTERSTATES_H

enum AI_LOGICSTATE {
    None,
    AlertedByPlayer,
    MovingToPlayer,
    ListeningToPlayer,
    AlertedByEnemy,
    MovingToEnemy,
    ListeningToEnemy,
};

enum AI_MOVEMENTSTATE {
    SetPathToPlayer,
    OnPathToPlayer,
    NearPlayerSubPoint,
    NearPlayerPosition,
    SetPathToEnemy,
    OnPathToEnemy,
    NearEnemySubPoint,
    NearEnemyPosition,
    OnPathToTarget,
    NearTargetSubPoint,
    NearTargetPosition,
    ReturnToPreviousTarget,
};

enum AI_ANIMATIONSTATE {
    Idle,
    Walking,
    Running,
    Cheering,
    Booing,
};

#endif //GLOOMENGINE_CHARACTERSTATES_H
