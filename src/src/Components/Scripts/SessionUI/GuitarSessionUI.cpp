#include "Components/Scripts/SessionUI/GuitarSessionUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

GuitarSessionUI::GuitarSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void GuitarSessionUI::Setup(std::shared_ptr<Instrument> instrument) {
    SessionUI::Setup(instrument);
    GameObject::Instantiate("Theme", parent)->AddComponent<Image>()
            ->LoadTexture(0, 0, "UI/Sesja/widokKlaskanie.png");
}

void GuitarSessionUI::PlaySound(int index) {
    // TODO to do
    SessionUI::PlaySound(index);
}

