#include "include/EngineManagers/HIDManager.h"
#include "include/GloomEngine.h"

HIDManager::HIDManager(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {
    glfwSetKeyCallback(gloomEngine->window, HIDManager::KeyActionCallback);
}
// TODO: need test
void HIDManager::Update() {
    keysDown.clear();
    keysPressed.clear();
    keysUp.clear();
    keysDown = keysDownBuffer;
    keysPressed = keysPressedBuffer;
    keysUp = keysUpBuffer;
    keysDownBuffer.clear();
    keysUpBuffer.clear();
}

bool HIDManager::IsKeyDown(Key key) {
    if (!keysDown.empty()) {
        for (int i = 0; i < keysDown.size(); i++) {
            if (keysDown[i] == key) {
                return true;
            }
        }
    }
    return false;
}

bool HIDManager::IsKeyPressed(Key key) {
    if (!keysPressed.empty()) {
        for (int i = 0; i < keysPressed.size(); i++) {
            if (keysPressed[i] == key) {
                return true;
            }
        }
    }
    return false;
}

bool HIDManager::IsKeyUp(Key key) {
    if (!keysUp.empty()) {
        for (int i = 0; i < keysUp.size(); i++) {
            if (keysUp[i] == key) {
                return true;
            }
        }
    }
    return false;
}

std::vector<Key> HIDManager::keysDownBuffer = {};
std::vector<Key> HIDManager::keysPressedBuffer = {};
std::vector<Key> HIDManager::keysUpBuffer = {};

void HIDManager::KeyActionCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Key down - first frame when clicked
    if (action == GLFW_PRESS) {
        keysDownBuffer.push_back(static_cast<Key>(key));
        keysPressedBuffer.push_back(static_cast<Key>(key));
    }
    // Key up - released
    if (action == GLFW_RELEASE) {
        if (!keysDownBuffer.empty()) {
            auto iterator = std::find(keysDownBuffer.begin(), keysDownBuffer.end(), static_cast<Key>(key));
            if(iterator != keysDownBuffer.end()) keysDownBuffer.erase(iterator);
        }
        if (!keysPressedBuffer.empty()) {
            auto iterator = std::find(keysPressedBuffer.begin(), keysPressedBuffer.end(), static_cast<Key>(key));
            if(iterator != keysPressedBuffer.end()) keysPressedBuffer.erase(iterator);
        }
        keysUpBuffer.push_back(static_cast<Key>(key));
    }
}

