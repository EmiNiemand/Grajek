#ifndef IMAGE_CPP_OPPONENT_H
#define IMAGE_CPP_OPPONENT_H

#include "Components/Component.h"

class Instrument;
class MusicPattern;
class PlayerManager;
class OpponentManager;
class AudioSource;
struct RawSample;
class Image;
class Dialogue;
class Button;
class OpponentSessionStarter;
class OpponentMusicSession;
class SessionUI;
enum InstrumentName;

inline static std::unordered_map<Key, int> PlaySound = {
        {Key::KEY_R, 0},
        {Key::KEY_U, 1},
        {Key::KEY_P, 2}
};

class Opponent : public Component {
private:
    std::shared_ptr<GameObject> ui;
    std::shared_ptr<Instrument> instrument;
    std::shared_ptr<MusicPattern> pattern;
    std::vector<std::shared_ptr<AudioSource>> sampleSources;
    std::shared_ptr<PlayerManager> playerManager;

    // Session Starter
    std::shared_ptr<Button> button1, button2;
    std::shared_ptr<OpponentSessionStarter> sessionStarter;

    std::shared_ptr<GameObject> chooseMenu;
    float battleTime = 60.0f;
    short sampleIndex = 0;
    float satisfaction = 0.0f;
    float timer = 0.0f;
    bool triggerActive = false;
    bool defeated = false;
    bool dialogueActive = false;
    bool chooseMenuActive = false;
    bool rejectDialogueActive = false;
    bool acceptDialogueActive = false;

public:
    // Music Session
    std::shared_ptr<OpponentMusicSession> musicSession;
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
    void Setup(std::shared_ptr<Instrument> instrument1, std::vector<RawSample> musicPattern, float satisfaction1);
    void Update() override;
    void UpdateSatisfaction(float satisfaction1);
    void CreateSessionStarter();
    void CreateMusicSession(InstrumentName instrumentName);
};

#endif //IMAGE_CPP_OPPONENT_H