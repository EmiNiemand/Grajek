#include "Components/Scripts/SessionUI/LaunchpadSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

LaunchpadSessionUI::LaunchpadSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void LaunchpadSessionUI::PlaySound(int index) {
    // TODO to do
    SessionUI::PlaySound(index);
}