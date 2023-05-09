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

std::vector<std::shared_ptr<StaticObjData>> FileDataHandler::LoadMap() {
    std::filesystem::path path(dataDirectoryPath);
    path /= dataFileName + ".json";
#ifdef DEBUG
    spdlog::info("Save path: " + path.string());
#endif

    std::vector<std::shared_ptr<StaticObjData>> mapData = {};

    try {
        std::ifstream input(path);

        nlohmann::json json;
        input >> json;
        from_json(json, mapData);
    }
    catch (std::exception e) {
        spdlog::info("Failed to read a file content at path: " + path.string());
    }

    return mapData;
}

void FileDataHandler::SaveMap(std::vector<std::shared_ptr<StaticObjData>> mapData) {
    std::filesystem::path path(dataDirectoryPath);
    path /= dataFileName + ".json";

#ifdef DEBUG
    spdlog::info("Save path: " + path.string());
#endif

    try {
        nlohmann::json json;
        to_json(json, mapData);

        std::ofstream saveFile(path);
        saveFile << json << std::endl;
    }
    catch(std::exception e) {
        spdlog::info("Failed to write a MAP file content at path: " + path.string());
    }
}

void FileDataHandler::to_json(nlohmann::json &json, std::vector<std::shared_ptr<StaticObjData>>& mapData) {
    json = nlohmann::json::array({});
    nlohmann::json objectJson;
    for (const auto& object: mapData){
        objectJson.clear();
        objectJson["position.x"] = object->position.x;
        objectJson["position.y"] = object->position.y;
        objectJson["position.z"] = object->position.z;

        objectJson["rotation.x"] = object->position.x;
        objectJson["rotation.y"] = object->position.y;
        objectJson["rotation.z"] = object->position.z;

        objectJson["scale.x"] = object->position.x;
        objectJson["scale.y"] = object->position.y;
        objectJson["scale.z"] = object->position.z;

        json.emplace(objectJson);
    }

}

void FileDataHandler::from_json(const nlohmann::json &json, std::vector<std::shared_ptr<StaticObjData>>& mapData) {
    mapData.clear();
    std::shared_ptr<StaticObjData> newObject;
for (const auto& object: json){
    newObject = std::make_shared<StaticObjData>();
    newObject->position.x = object["position.x"];
    newObject->position.y = object["position.y"];
    newObject->position.z = object["position.z"];

    newObject->rotation.x = object["rotation.x"];
    newObject->rotation.y = object["rotation.y"];
    newObject->rotation.z = object["rotation.z"];

    newObject->scale.x = object["position.x"];
    newObject->scale.y = object["position.y"];
    newObject->scale.z = object["position.z"];
    mapData.push_back(newObject);
}
}

