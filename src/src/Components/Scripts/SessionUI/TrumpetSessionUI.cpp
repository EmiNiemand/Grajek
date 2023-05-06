#include "Components/Scripts/SessionUI/TrumpetSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

TrumpetSessionUI::TrumpetSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void TrumpetSessionUI::PlaySound(int index) {
    // TODO to do
    SessionUI::PlaySound(index);
}