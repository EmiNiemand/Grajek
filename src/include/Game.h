#ifndef OPENGLGP_GAME_H
#define OPENGLGP_GAME_H

#include <memory>

class GloomEngine;
class GameObject;

class Game {
public:
    std::shared_ptr<GameObject> activeCamera;
    std::shared_ptr<GameObject> activeScene;
    bool shouldQuit = false;

public:
    explicit Game();
    virtual ~Game();

    /// Should be used to load main menu scene
    void InitializeGame() const;
    bool GameLoop();
};


#endif //OPENGLGP_GAME_H
