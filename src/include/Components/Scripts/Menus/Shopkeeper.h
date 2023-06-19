#ifndef IMAGE_CPP_SHOPKEEPER_H
#define IMAGE_CPP_SHOPKEEPER_H

#include "Components/Component.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Interfaces/IDataPersistance.h"
#include "Components/Audio/AudioSource.h"
#include <vector>

class Image;
class Text;
class PlayerManager;
class UIAnimator;

class Shopkeeper : public Component, public IDataPersistance {
private:
    std::vector<std::shared_ptr<AudioSource>> sampleSources;
    std::vector<std::shared_ptr<Image>> sampleImages;
    std::vector<std::vector<std::shared_ptr<UIAnimator>>> sampleAnimators;

    std::shared_ptr<Image> background;
    std::shared_ptr<Image> clapIcon;
    std::shared_ptr<Image> image;
    std::shared_ptr<Text> text1;
    std::shared_ptr<Text> text2;
    std::shared_ptr<Text> text3;
    std::shared_ptr<GameObject> dialogue;
    std::shared_ptr<GameObject> shopkeeperModel;
    std::shared_ptr<GameObject> door;
    std::shared_ptr<PlayerManager> playerManager;
    std::shared_ptr<Image> instrumentControl;
    std::shared_ptr<Image> cheatSheet;
    short dialogueIndex = 0;
    short pattern = 0;
    bool shopkeeperEvent = false;
    bool instrumentControlActive = false;
    bool cheatSheetActive = false;

public:
    std::vector<Strings> texts;

public:
    Shopkeeper(const std::shared_ptr<GameObject> &parent, int id);
    ~Shopkeeper() override;

    void Start() override;
    void Update() override;

    void OnDestroy() override;

    void NextDialogue();
    void LoadData(std::shared_ptr<GameData> data) override;
    void SaveData(std::shared_ptr<GameData> &data) override;
};

#endif //IMAGE_CPP_SHOPKEEPER_H