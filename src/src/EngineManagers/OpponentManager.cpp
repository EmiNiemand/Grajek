#include "EngineManagers/OpponentManager.h"
#include "Components/Scripts/Opponent.h"
#include "Components/UI/Image.h"

OpponentManager::OpponentManager() = default;

OpponentManager::~OpponentManager() {
    delete opponentManager;
}

OpponentManager *OpponentManager::GetInstance() {
    if (opponentManager == nullptr) {
        opponentManager = new OpponentManager();
    }
    return opponentManager;
}

void OpponentManager::NotifyPlayerPlayedPattern(float satisfaction) const {
    for (const auto & opponent : opponents) {
        opponent.second->UpdateSatisfaction(satisfaction);
    }
}

void OpponentManager::NotifyPlayerStopsPlaying() {
    for (const auto & opponent : opponents) {
        opponent.second->wellPlayedPatternCount = 0;
        opponent.second->satisfactionAverage = 0;
        for (const auto & i : opponent.second->belt) {
            i->enabled = false;
        }
    }
}