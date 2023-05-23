#ifndef IMAGE_CPP_DIALOGUE_H
#define IMAGE_CPP_DIALOGUE_H

#include "Components/Component.h"
#include <vector>

class Image;
class Text;
class PlayerManager;

struct Strings {
    std::string text1;
    std::string text2;
    std::string text3;
};

class Dialogue : public Component {
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
    Dialogue(const std::shared_ptr<GameObject> &parent, int id);
    ~Dialogue() override;

    void Start() override;
    void OnTriggerEnter(const std::shared_ptr<GameObject>& gameObject) override;
    void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject) override;
    void Update() override;
    void ShowDialogue();
    void HideDialogue();
};

#endif //IMAGE_CPP_DIALOGUE_H