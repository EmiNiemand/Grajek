#ifndef IMAGE_CPP_SHOPKEEPER_H
#define IMAGE_CPP_SHOPKEEPER_H

#include "Components/Component.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include <vector>

class Image;
class Text;
class PlayerManager;

class Shopkeeper : public Component {
private:
    bool triggerActive = false;
    bool active = false;
    std::shared_ptr<Image> image;
    std::shared_ptr<Text> text1;
    std::shared_ptr<Text> text2;
    std::shared_ptr<Text> text3;
    std::shared_ptr<GameObject> dialogue;
    std::shared_ptr<PlayerManager> playerManager;
    short dialogueIndex = 0;

public:
    std::vector<Strings> texts;

public:
    Shopkeeper(const std::shared_ptr<GameObject> &parent, int id);
    ~Shopkeeper() override;

    void Start() override;
    void OnTriggerEnter(const std::shared_ptr<GameObject>& gameObject) override;
    void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject) override;
    void Update() override;
    void ShowDialogue();
    void HideDialogue();
};

#endif //IMAGE_CPP_SHOPKEEPER_H