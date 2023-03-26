//
// Created by szymo on 24/03/2023.
//

#include "LowLevelClasses/FileDataHandler.h"
#include "LowLevelClasses/GameData.h"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <iostream>
#include <fstream>


FileDataHandler::FileDataHandler(const std::string &dataDirectoryPath, const std::string &dataFileName)
        : dataDirectoryPath(dataDirectoryPath), dataFileName(dataFileName) {}

FileDataHandler::~FileDataHandler() {}

std::shared_ptr<GameData> FileDataHandler::LoadGame() {
    std::filesystem::path path(dataDirectoryPath);
    path /= dataFileName + ".json";

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
    //TODO: add instruments later on
    json["money"] = gameData->money;
    json["reputation"] = gameData->reputation;
    json["playerPosition.x"] = gameData->playerPosition.x;
    json["playerPosition.y"] = gameData->playerPosition.y;
    json["playerPosition.z"] = gameData->playerPosition.z;
}

void FileDataHandler::from_json(const nlohmann::json &json, const std::shared_ptr<GameData>& gameData) {
    //TODO: add instruments later on
    json.at("money").get_to(gameData->money);
    json.at("reputation").get_to(gameData->reputation);
    json.at("playerPosition.x").get_to(gameData->playerPosition.x);
    json.at("playerPosition.y").get_to(gameData->playerPosition.y);
    json.at("playerPosition.z").get_to(gameData->playerPosition.z);
}

