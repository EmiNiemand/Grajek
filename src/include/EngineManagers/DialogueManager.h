#ifndef IMAGE_CPP_DIALOGUEMANAGER_H
#define IMAGE_CPP_DIALOGUEMANAGER_H

#include <vector>
#include <memory>

class Dialogue;

class DialogueManager {
private:
    inline static DialogueManager* dialogueManager;

public:
    std::vector<std::shared_ptr<Dialogue>> dialogues;

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