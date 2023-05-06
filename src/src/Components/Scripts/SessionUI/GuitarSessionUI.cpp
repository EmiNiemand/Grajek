#include "Components/Scripts/SessionUI/GuitarSessionUI.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

GuitarSessionUI::GuitarSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void GuitarSessionUI::PlaySound(int index) {
    // TODO to do
    SessionUI::PlaySound(index);
}

