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
class AudioSource;

class Shopkeeper : public Component, public IDataPersistance {
private:
    std::shared_ptr<Image> spaceImage;
    std::shared_ptr<Image> musicSessionImage;
    std::shared_ptr<Image> instrumentControl;
    std::shared_ptr<Image> instrumentControlImage;
    std::shared_ptr<Image> patterns;
    std::shared_ptr<Image> patternsImage;
    std::shared_ptr<AudioSource> patternsSound;
    std::shared_ptr<Image> soundButton1, soundButton2;
    std::shared_ptr<Image> soundImage;
    std::shared_ptr<Image> stopMusicSessionImage;
    std::shared_ptr<Image> crowdImage;

    std::shared_ptr<Image> image;
    std::shared_ptr<Text> text1;
    std::shared_ptr<Text> text2;
    std::shared_ptr<Text> text3;
    std::shared_ptr<GameObject> dialogue;
    std::shared_ptr<GameObject> shopkeeperModel;
    std::shared_ptr<GameObject> door;
    std::shared_ptr<PlayerManager> playerManager;
    short dialogueIndex = 0;
    short pattern = 0;
    bool patternIsGood = false;
    bool shopkeeperEvent = false;
    bool tutorial = false;

public:
    std::vector<Strings> texts;
    bool menuActive = false;

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