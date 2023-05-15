//
// Created by masterktos on 06.04.23.
//

#ifndef GLOOMENGINE_SESSIONUI_H
#define GLOOMENGINE_SESSIONUI_H


#include <string>
#include <utility>
#include <vector>
#include <map>
#include "Components/Component.h"
#include "Components/Scripts/MusicPattern.h"
#include "Utilities.h"

class Image;
class Text;
class GameObject;
class AudioSource;
class UIAnimator;

class SessionUI : public Component {
private:
    std::shared_ptr<Text> accuracyFeedback;
    inline static glm::vec3 accuracyColors[]  { Color::Red, Color::Cyan, Color::Green, Color::Yellow };
    inline static std::string accuracyTexts[] { "Poor", "Nice", "Great!", "PERFECT" };
    inline static float accuracyThresholds[] { 0.5f, 0.8f, 0.95f };
    std::vector<std::shared_ptr<Image>> accuracyRating;
    std::vector<std::shared_ptr<UIAnimator>> accuracyRatingAnimator;

    bool metronomeSoundEnabled;
    bool metronomeVisualEnabled;
    std::shared_ptr<Image> metronomeImage;
    std::shared_ptr<AudioSource> tickSound;

    std::shared_ptr<Image> cheatSheet;
    bool cheatSheetActive = false;
    std::vector<std::shared_ptr<AudioSource>> sampleSources;

public:
    SessionUI(const std::shared_ptr<GameObject> &parent, int id);

    void Setup(int bpm, const std::vector<std::shared_ptr<Sample>> &samples, std::string metronomePath);
    void SetCheatSheet(std::string cheatSheetPath);

    inline virtual void PlaySound(int index);
    void ToggleCheatSheet();
    // Fraction values: <0, 1>
    void UpdateAccuracy(float fraction);

    void Update() override;

private:
    void MetronomeSetup(const std::string& metronomePath, int bpm);
    void AccuracyFeedbackSetup();
};


#endif //GLOOMENGINE_SESSIONUI_H
