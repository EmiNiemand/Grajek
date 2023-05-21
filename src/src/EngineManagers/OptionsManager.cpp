#include "EngineManagers/OptionsManager.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <utility>
#include "nlohmann/json.hpp"
#include <spdlog/spdlog.h>

OptionsManager::~OptionsManager() = default;

OptionsManager *OptionsManager::GetInstance() {
    if (optionsManager == nullptr) {
        optionsManager = new OptionsManager();
    }
    return optionsManager;
}

OptionsManager::OptionsManager() {
    musicVolume = 1.0f;
    width = 1440;
    height = 810;
    fullScreen = false;
    shadowResolution = 4096;
}

void OptionsManager::Load() {
    std::filesystem::path path = std::filesystem::current_path().parent_path().parent_path();
    path /= "res";
    path /= "ProjectConfig";
    path /= "config.json";

    try {
        std::ifstream input(path);

        nlohmann::json json;
        input >> json;
        json.at("musicVolume").get_to(musicVolume);
        json.at("width").get_to(width);
        json.at("height").get_to(height);
        json.at("fullScreen").get_to(fullScreen);
        json.at("shadowResolution").get_to(shadowResolution);
    }
    catch (std::exception e) {
        spdlog::info("Failed to read a file content at path: " + path.string());
    }
}

void OptionsManager::Save() {
    std::filesystem::path path = std::filesystem::current_path().parent_path().parent_path();
    path /= "res";
    path /= "ProjectConfig";
    path /= "config.json";

    try {
        std::ofstream output(path);

        nlohmann::json json;
        json["musicVolume"] = musicVolume;
        json["width"] = width;
        json["height"] = height;
        json["fullScreen"] = fullScreen;
        json["shadowResolution"] = shadowResolution;

        std::ofstream saveFile(path);
        saveFile << json << std::endl;
    }
    catch (std::exception e) {
        spdlog::info("Failed to read a file content at path: " + path.string());
    }
}