//
// Created by Adrian on 04.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERSTATES_H
#define GLOOMENGINE_CHARACTERSTATES_H

enum AI_LOGIC_STATE {
    Wandering,
    AlertedByPlayer,
    MovingToPlayer,
    ListeningToPlayer,
    AlertedByPlayerTalking,
    AlertedByEnemy,
    MovingToEnemy,
    ListeningToEnemy,
    WalkingAway,
};

enum AI_MOVEMENT_STATE {
    Waiting,
    SettingPathToPlayer,
    OnPathToPlayer,
    NearPlayerPosition,
    SettingPathToEnemy,
    OnPathToEnemy,
    NearEnemyPosition,
    OnPathToTarget,
    NearTargetPosition,
    ReturningToPreviousTarget,
};

enum AI_ANIMATION_STATE {
    Idle,
    Walking,
    Running,
    Cheering,
    Booing,
};

#endif //GLOOMENGINE_CHARACTERSTATES_H
