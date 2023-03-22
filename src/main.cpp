#include "include/GloomEngine.h"

int main(int, char**)
{
    std::shared_ptr<GloomEngine> gloomEngine = std::make_shared<GloomEngine>();
    gloomEngine->Initialize();
    gloomEngine->Awake();
    gloomEngine->Start();

    bool endGame = false;

    // ________________________
    // ---------WHILE----------
    // ________________________
    while (!endGame)
    {
        endGame = gloomEngine->Update();
    }

    gloomEngine->Free();
    return 0;
}
