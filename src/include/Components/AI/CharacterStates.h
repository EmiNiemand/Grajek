//
// Created by Adrian on 04.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERSTATES_H
#define GLOOMENGINE_CHARACTERSTATES_H

enum AI_LOGIC_STATE {
    Wandering,
    AlertedByPlayer,
    AlertedByOpponent,
    MovingToPlayer,
    MovingToDuel,
    ListeningToPlayer,
    ListeningToDuel,
    WalkingAway,
};

enum AI_MOVEMENT_STATE {
    Stuck,
    Waiting,
    SettingPathToDuel,
    OnPathToDuel,
    NearDuelPosition,
    SettingPathToPlayer,
    OnPathToPlayer,
    NearPlayerPosition,
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
