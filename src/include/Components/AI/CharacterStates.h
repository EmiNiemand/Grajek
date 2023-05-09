//
// Created by Adrian on 04.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERSTATES_H
#define GLOOMENGINE_CHARACTERSTATES_H

enum AI_LOGICSTATE {
    RunningToPlayer,
    WalkingOnPath,
    AlertedByPlayer
};

enum AI_ANIMATIONSTATE {
    Idle,
    Walking,
    Running,
    Cheering,
    Booing
};

#endif //GLOOMENGINE_CHARACTERSTATES_H
