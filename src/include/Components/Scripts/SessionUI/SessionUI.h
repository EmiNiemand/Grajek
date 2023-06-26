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
class Button;
class Instrument;

class SessionUI : public Component {
private:
    // Accuracy rating
    std::vector<std::shared_ptr<Image>> accuracyRating;
    std::vector<std::shared_ptr<UIAnimator>> accuracyRatingAnimator;
    inline static float accuracyThresholds[] { 0.5f, 0.8f, 0.95f };

    // Metronome
    std::shared_ptr<Image> metronomeImage;
    std::shared_ptr<Image> metronomeMask;
    std::shared_ptr<UIAnimator> metronomeAnimator;
    std::shared_ptr<AudioSource> tickSound;
    std::shared_ptr<Image> metronomeSoundIndicator;
    std::shared_ptr<Image> metronomeVisualsIndicator;
    bool metronomeSoundEnabled;
    bool metronomeVisualEnabled;

    // Backing track
    std::shared_ptr<AudioSource> backingTrack;
    std::shared_ptr<Image> backingTrackIndicator;
    bool backingTrackEnabled = true;

    // Instrument control
    std::shared_ptr<Image> instrumentControl;
    bool instrumentControlActive = false;

    std::shared_ptr<AudioSource> notesRevealSound;
    std::shared_ptr<AudioSource> notesHideSound;

    // Cheat sheet
    std::shared_ptr<Image> cheatSheet;

protected:
    bool cheatSheetActive = false;

    std::shared_ptr<Button> activeButton;
    std::vector<std::shared_ptr<Button>> soundButtons;
    std::vector<std::vector<std::shared_ptr<UIAnimator>>> soundAnimators;
    std::vector<std::shared_ptr<AudioSource>> patternsSounds;

    // Samples
    std::vector<std::shared_ptr<AudioSource>> sampleSources;
    std::vector<std::shared_ptr<Image>> sampleImages;
    std::vector<std::vector<std::shared_ptr<UIAnimator>>> sampleAnimators;


public:
    SessionUI(const std::shared_ptr<GameObject> &parent, int id);

    // In child classes you need to place BackingTrackSetup() before calling parent's Setup()
    virtual void Setup(std::shared_ptr<Instrument> instrument);
    void SetCheatSheet(const std::string& cheatSheetPath);
    void SetInstrumentControl(const std::string& instrumentControlPath);
    void SetTheme(const std::string& themePath);

    virtual void PlaySound(int index);
    virtual void StopSound(int index);
    virtual bool ToggleCheatSheet();
    void ToggleInstrumentControl();
    // Fraction values: <0, 1>
    void UpdateAccuracy(float fraction);

    void Update() override;

    void OnDestroy() override;

    bool ToggleMetronomeSound();
    bool ToggleMetronomeVisuals();
    bool ToggleBackingTrack();

    void ChangeActiveButton(glm::vec2 moveVector);
    void OnClick();

protected:
    void BackingTrackSetup(const std::string& trackName);
private:
    void MetronomeSetup(int bpm);
    void AccuracyFeedbackSetup();
};


#endif //GLOOMENGINE_SESSIONUI_H
