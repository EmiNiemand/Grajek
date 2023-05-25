#include "EngineManagers/OpponentManager.h"
#include "Components/Scripts/Opponent.h"
#include "Components/UI/Image.h"

OpponentManager::OpponentManager() = default;

OpponentManager::~OpponentManager() = default;

OpponentManager *OpponentManager::GetInstance() {
    if (opponentManager == nullptr) {
        opponentManager = new OpponentManager();
    }
    return opponentManager;
}

void OpponentManager::NotifyPlayerPlayedPattern(float satisfaction) const {
    for (const auto & opponent : opponents) {
        opponent->UpdateSatisfaction(satisfaction);
    }
}

void OpponentManager::NotifyPlayerStopsPlaying() {
    for (const auto & opponent : opponents) {
        opponent->wellPlayedPatternCount = 0;
        opponent->satisfactionAverage = 0;
        for (const auto & i : opponent->belt) {
            i->enabled = false;
        }
    }
}