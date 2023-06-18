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
    std::shared_ptr<PlayerManager> playerManager;

    // Session Starter
    std::shared_ptr<Button> button1, button2;

    std::shared_ptr<GameObject> chooseMenu;
    PlayerBadges badge;

    float battleTime = 60.0f;
    float satisfaction = 0.0f;
    float timer = 0.0f;
    short sampleIndex = 0;
    short bet = 0;
    bool defeated = false;
    bool dialogueActive = false;
    bool chooseMenuActive = false;
    bool rejectDialogueActive = false;
    bool acceptDialogueActive = false;
    bool sessionStarter = false;
    bool musicSession = false;

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

public:
    Opponent(const std::shared_ptr<GameObject> &parent, int id);
    ~Opponent() override;

    void OnDestroy() override;
    void Start() override;
    void Setup(std::shared_ptr<Instrument> instrument1, std::vector<RawSample> musicPattern, float satisfaction1,
               short bet1, PlayerBadges badge1 = (PlayerBadges)(-1));
    void Update() override;
    void PlayerPlayedPattern(float satisfaction1);
    void PlayerStartedMusicSession();
};

#endif //IMAGE_CPP_OPPONENT_H