//
// Created by szymo on 13/03/2023.
//

#include "include/EngineComponents/EngineHID.h"
#include "include/GloomEngine.h"

EngineHID::EngineHID(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {
    glfwSetKeyCallback(gloomEngine->window, EngineHID::KeyActionCallback);
}

void EngineHID::Update() {
    spdlog::info("Up: " + std::to_string(keysUp.size()));
    spdlog::info("Pressed: " + std::to_string(keysPressed.size()));
    spdlog::info("Down: " + std::to_string(keysDown.size()));
    for (int i = 0; i < keysDown.size(); i++) {
        bool exist = false;
        for (int j = 0; j < keysPressed.size(); j++) {
            if (keysPressed[j] == keysDown[i]) {
                exist = true;
            }
        }
        if (!exist) keysPressed.push_back(keysDown[i]);
    }
    keysDown.clear();
    keysUp.clear();
}

bool EngineHID::IsKeyDown(Key key) {
    if (!keysDown.empty()) {
        for (int i = 0; i < keysDown.size(); i++) {
            if (keysDown[i] == key) {
                return true;
            }
        }
    }
    return false;
}

bool EngineHID::IsKeyPressed(Key key) {
    if (!keysPressed.empty()) {
        for (int i = 0; i < keysPressed.size(); i++) {
            if (keysPressed[i] == key) {
                return true;
            }
        }
    }
    return false;
}

bool EngineHID::IsKeyUp(Key key) {
    if (!keysUp.empty()) {
        for (int i = 0; i < keysUp.size(); i++) {
            if (keysUp[i] == key) {
                return true;
            }
        }
    }
    return false;
}

std::vector<Key> EngineHID::keysDown = {};
std::vector<Key> EngineHID::keysPressed = {};
std::vector<Key> EngineHID::keysUp = {};

void EngineHID::KeyActionCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Key down - first frame when clicked
    if (action == GLFW_PRESS) {
        bool exist = false;
        for (int i = 0; i < keysDown.size(); i++) {
            if (keysDown[i] == static_cast<Key>(key)) {
                exist = true;
            }
        }
        for (int i = 0; i < keysPressed.size(); i++) {
            if (keysPressed[i] == static_cast<Key>(key)) {
                exist = true;
            }
        }
        if (!exist) keysDown.push_back(static_cast<Key>(key));
    }
    // Key up - released
    if (action == GLFW_RELEASE) {
        if (!keysDown.empty()) {
            for (int i = 0; i < keysDown.size(); i++) {
                if (keysDown[i] == static_cast<Key>(key)) {
                    keysDown.erase(keysDown.begin() + i);
                }
            }
        }
        if (!keysPressed.empty()) {
            for (int i = 0; i < keysPressed.size(); i++) {
                if (keysPressed[i] == static_cast<Key>(key)) {
                    keysPressed.erase(keysPressed.begin() + i);
                }
            }
        }
        keysUp.push_back(static_cast<Key>(key));
    }
}

