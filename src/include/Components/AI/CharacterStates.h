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
    WalkingAway,
};

enum AI_MOVEMENTSTATE {
    SettingPathToPlayer,
    OnPathToPlayer,
    NearPlayerSubPoint,
    NearPlayerPosition,
    SettingPathToEnemy,
    OnPathToEnemy,
    NearEnemySubPoint,
    NearEnemyPosition,
    OnPathToTarget,
    NearTargetSubPoint,
    NearTargetPosition,
    ReturningToPreviousTarget,
};

enum AI_ANIMATIONSTATE {
    Idle,
    Walking,
    Running,
    Cheering,
    Booing,
};

#endif //GLOOMENGINE_CHARACTERSTATES_H
