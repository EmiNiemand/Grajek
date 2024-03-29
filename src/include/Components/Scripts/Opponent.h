#ifndef IMAGE_CPP_OPPONENT_H
#define IMAGE_CPP_OPPONENT_H

#include "Components/Component.h"
#include <vector>

class Instrument;
class MusicPattern;
class PlayerManager;
class AudioSource;
struct RawSample;
class Image;
class Dialogue;
class Button;
class SessionUI;
enum InstrumentName;

class Opponent : public Component {
private:
    std::shared_ptr<GameObject> ui;
    std::shared_ptr<Instrument> instrument;
    std::shared_ptr<MusicPattern> pattern;
    std::vector<std::shared_ptr<AudioSource>> sampleSources;
    std::shared_ptr<AudioSource> winSound;
    std::shared_ptr<AudioSource> loseSound;
    std::shared_ptr<PlayerManager> playerManager;

    // Session Starter
    std::shared_ptr<Button> button1, button2;

    std::shared_ptr<GameObject> chooseMenu;
    PlayerBadges badge = (PlayerBadges)(-1);

    float battleTime = 30.0f;
    float patternDelay = 0.0f;
    float accuracy = 0.0f;
    float timer = 0.0f;
    float patternTimer = 0.0f;
    float startPlayingTimer = 1.0f;
    short sampleIndex = 0;
    short bet = 0;
    bool defeated = false;
    bool dialogueActive = false;
    bool chooseMenuActive = false;
    bool rejectDialogueActive = false;
    bool acceptDialogueActive = false;
    bool sessionStarter = false;
    bool musicSession = false;
    bool shouldPlay = true;

public:
    // Music Session
    std::shared_ptr<Image> belt;
    std::shared_ptr<Image> timeCounter;
    float time = 0.0f;
    float satisfactionDifference = 0.0f;

    std::shared_ptr<Dialogue> dialogue;
    std::shared_ptr<Dialogue> winDialogue;
    std::shared_ptr<Dialogue> lossDialogue;
    std::shared_ptr<SessionUI> sessionUI;
    std::shared_ptr<GameObject> indicator;
    std::string name;

public:
    Opponent(const std::shared_ptr<GameObject> &parent, int id);
    ~Opponent() override;

    void OnDestroy() override;
    void Start() override;
    void Setup(std::shared_ptr<Instrument> instrument1, float patternDelay1, float accuracy1, short bet1,
               glm::vec3 indicatorColor = glm::vec3(1, 1, 0), PlayerBadges badge1 = (PlayerBadges)(-1));
    void Update() override;
    void PlayerPlayedPattern(float satisfaction1);
    void PlayerStartedMusicSession();
};

#endif //IMAGE_CPP_OPPONENT_H