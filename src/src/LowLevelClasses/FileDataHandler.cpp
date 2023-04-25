//
// Created by szymo on 24/03/2023.
//

#include "LowLevelClasses/FileDataHandler.h"
#include "LowLevelClasses/GameData.h"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <utility>


FileDataHandler::FileDataHandler(std::string dataDirectoryPath, std::string dataFileName)
        : dataDirectoryPath(std::move(dataDirectoryPath)), dataFileName(std::move(dataFileName)) {}

FileDataHandler::~FileDataHandler() = default;

std::shared_ptr<GameData> FileDataHandler::LoadGame() {
    std::filesystem::path path(dataDirectoryPath);
    path /= dataFileName + ".json";
#ifdef DEBUG
    spdlog::info("Save path: " + path.string());
#endif

    std::shared_ptr<GameData> gameData = std::make_shared<GameData>();

    try {
        std::ifstream input(path);

        nlohmann::json json;
        input >> json;
        from_json(json, gameData);
    }
    catch (std::exception e) {
        spdlog::info("Failed to read a file content at path: " + path.string());
    }

    return gameData;
}

void FileDataHandler::SaveGame(std::shared_ptr<GameData> gameData) {
    std::filesystem::path path(dataDirectoryPath);
    path /= dataFileName + ".json";

#ifdef DEBUG
    spdlog::info("Save path: " + path.string());
#endif

    try {
        nlohmann::json json;
        to_json(json, gameData);

        std::ofstream saveFile(path);
        saveFile << json << std::endl;
    }
    catch(std::exception e) {
        spdlog::info("Failed to write a file content at path: " + path.string());
    }
}

void FileDataHandler::to_json(nlohmann::json &json, const std::shared_ptr<GameData>& gameData) {
    json["money"] = gameData->money;
    json["reputation"] = gameData->reputation;
    json["playerPosition.x"] = gameData->playerPosition.x;
    json["playerPosition.y"] = gameData->playerPosition.y;
    json["playerPosition.z"] = gameData->playerPosition.z;
    if(gameData->instruments.contains(InstrumentName::Drums)) json["instrument.Drums"] = 0;
    if(gameData->instruments.contains(InstrumentName::Trumpet)) json["instrument.Trumpet"] = 0;
    if(gameData->instruments.contains(InstrumentName::Launchpad)) json["instrument.Launchpad"] = 0;
    if(gameData->instruments.contains(InstrumentName::Guitar)) json["instrument.Guitar"] = 0;
}

void FileDataHandler::from_json(const nlohmann::json &json, const std::shared_ptr<GameData>& gameData) {
    json.at("money").get_to(gameData->money);
    json.at("reputation").get_to(gameData->reputation);
    json.at("playerPosition.x").get_to(gameData->playerPosition.x);
    json.at("playerPosition.y").get_to(gameData->playerPosition.y);
    json.at("playerPosition.z").get_to(gameData->playerPosition.z);
    if(json.contains("instrument.Drums")) gameData->instruments.insert(InstrumentName::Drums);
    if(json.contains("instrument.Trumpet")) gameData->instruments.insert(InstrumentName::Trumpet);
    if(json.contains("instrument.Launchpad")) gameData->instruments.insert(InstrumentName::Launchpad);
    if(json.contains("instrument.Guitar")) gameData->instruments.insert(InstrumentName::Guitar);
}

