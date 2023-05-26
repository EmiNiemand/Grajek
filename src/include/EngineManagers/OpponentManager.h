#ifndef IMAGE_CPP_OPPONENTMANAGER_H
#define IMAGE_CPP_OPPONENTMANAGER_H

#include <vector>
#include <memory>

class Opponent;

class OpponentManager {
private:
    inline static OpponentManager* opponentManager;

public:
    std::vector<std::shared_ptr<Opponent>> opponents;

private:
    explicit OpponentManager();

public:
    OpponentManager(OpponentManager &other) = delete;
    void operator=(const OpponentManager&) = delete;
    virtual ~OpponentManager();

    static OpponentManager* GetInstance();

    void NotifyPlayerPlayedPattern(float satisfaction) const;
    void NotifyPlayerStopsPlaying();
};

#endif //IMAGE_CPP_OPPONENTMANAGER_H