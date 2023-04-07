//
// Created by masterktos on 06.04.23.
//

#ifndef GLOOMENGINE_SESSIONUI_H
#define GLOOMENGINE_SESSIONUI_H


#include <string>
#include <utility>
#include <vector>
#include "Components/Component.h"

class Image;
class Text;
class MusicPattern;
class GameObject;
class Sample;

class SessionUI : public Component {
private:
    std::shared_ptr<Text> accuracyFeedback;
//    inline static Color[] accuracyColors = { Color.red, Color.cyan, Color.green, Color.yellow };
    inline static std::string accuracyTexts[] { "Poor", "Nice", "Great!", "PERFECT" };
    inline static float accuracyThresholds[] { 0.5f, 0.8f, 0.95f };

    bool metronomeSoundEnabled;
    bool metronomeVisualEnabled;
    std::shared_ptr<Image> metronomeImage;

    std::shared_ptr<Image> cheatSheet;
//    std::vector<std::shared_ptr<AudioSource>> sampleSources;

public:
    SessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void Setup(int bpm, std::vector<std::shared_ptr<Sample>> samples, std::shared_ptr<Image> metronomeImage);
    void SetCheatSheet(std::shared_ptr<Image> newCheatSheet);

    void PlaySound(int index);
    void ToggleCheatSheet();
    // Fraction values: <0, 1>
    void UpdateAccuracy(float fraction);
};


#endif //GLOOMENGINE_SESSIONUI_H
