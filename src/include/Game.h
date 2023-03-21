#ifndef OPENGLGP_GAME_H
#define OPENGLGP_GAME_H

#include <memory>

class GloomEngine;
class GameObject;

class Game {
public:
    std::shared_ptr<GloomEngine> gloomEngine;
    std::shared_ptr<GameObject> activeCamera;
    std::shared_ptr<GameObject> activeScene;

public:
    explicit Game(const std::shared_ptr<GloomEngine> &gloomEngine);
    virtual ~Game();

    void Init();
    void Awake();
    void Start();
    bool Update();
};


#endif //OPENGLGP_GAME_H
