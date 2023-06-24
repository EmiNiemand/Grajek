#include "Components/Scripts/SessionUI/LaunchpadSessionUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"

LaunchpadSessionUI::LaunchpadSessionUI(const std::shared_ptr<GameObject> &parent, int id) : SessionUI(parent, id) {}

void LaunchpadSessionUI::Setup(std::shared_ptr<Instrument> instrument, bool sessionMetronomeSound,
                               bool sessionMetronomeVisuals, bool sessionBackingTrack) {
    SessionUI::Setup(instrument, sessionMetronomeSound, sessionMetronomeVisuals, sessionBackingTrack);
    GameObject::Instantiate("Theme", parent)->AddComponent<Image>()
        ->LoadTexture(0, 0, "UI/Sesja/widokLaunchpad.png");
}

void LaunchpadSessionUI::PlaySound(int index) {
    // TODO to do
    SessionUI::PlaySound(index);
}