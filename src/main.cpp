#include "GloomEngine.h"

int main(int, char**)
{
    GloomEngine::GetInstance()->Initialize();
    GloomEngine::GetInstance()->Awake();
    GloomEngine::GetInstance()->Start();

    bool endGame = false;

    // ________________________
    // ---------WHILE----------
    // ________________________
    while (!endGame)
    {
        endGame = GloomEngine::GetInstance()->Update();
    }

    GloomEngine::GetInstance()->Free();
    return 0;
}
