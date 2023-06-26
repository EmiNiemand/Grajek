#ifndef IMAGE_CPP_DIALOGUEMANAGER_H
#define IMAGE_CPP_DIALOGUEMANAGER_H

#include <memory>
#include <unordered_map>

class Dialogue;
class Shopkeeper;

class DialogueManager {
private:
    inline static DialogueManager* dialogueManager;

public:
    std::unordered_map<int, std::shared_ptr<Dialogue>> dialogues;
    std::shared_ptr<Shopkeeper> shopkeeper;

private:
    explicit DialogueManager();

public:
    DialogueManager(DialogueManager &other) = delete;
    void operator=(const DialogueManager&) = delete;
    virtual ~DialogueManager();

    static DialogueManager* GetInstance();

    void NotifyMenuIsActive();
    void NotifyMenuIsNotActive();
};

#endif //IMAGE_CPP_DIALOGUEMANAGER_H