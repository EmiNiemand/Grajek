//
// Created by szymo on 24/03/2023.
//

#include "EngineManagers/DataPersistanceManager.h"
#include "GloomEngine.h"
#include "LowLevelClasses/GameData.h"
#include "LowLevelClasses/FileDataHandler.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Component.h"
#include "Interfaces/IDataPersistance.h"

DataPersistanceManager::DataPersistanceManager() = default;

DataPersistanceManager::~DataPersistanceManager() {
    delete dataPersistanceManager;
}

DataPersistanceManager* DataPersistanceManager::GetInstance() {
    if (dataPersistanceManager == nullptr) {
        dataPersistanceManager = new DataPersistanceManager();
    }
    return dataPersistanceManager;
}

void DataPersistanceManager::NewGame() {
    gameData = std::make_unique<GameData>();
}

void DataPersistanceManager::LoadGame(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    FileDataHandler fileDataHandler(dataDirectoryPath, dataFileName);
    gameData = fileDataHandler.LoadGame();

    std::vector<std::shared_ptr<IDataPersistance>> dataPersistanceObjects = FindAllDataPersistanceObjects();

    for (const auto& object : dataPersistanceObjects) {
        object->LoadData(gameData);
    }
#ifdef DEBUG
    spdlog::info("Loading game: " + std::to_string(gameData->money)
                 + ", [" + std::to_string(gameData->playerPosition.x) + ", " + std::to_string(gameData->playerPosition.y)
                 + ", " + std::to_string(gameData->playerPosition.z) + "]");
#endif
}

void DataPersistanceManager::SaveGame(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    std::vector<std::shared_ptr<IDataPersistance>> dataPersistanceObjects = FindAllDataPersistanceObjects();

    for (const auto& object : dataPersistanceObjects) {
        object->SaveData(gameData);
    }

    FileDataHandler fileDataHandler(dataDirectoryPath, dataFileName);
    fileDataHandler.SaveGame(gameData);
#ifdef DEBUG
    spdlog::info("Saving game: " + std::to_string(gameData->money)
    + ", [" + std::to_string(gameData->playerPosition.x) + ", " + std::to_string(gameData->playerPosition.y)
    + ", " + std::to_string(gameData->playerPosition.z) + "]");
#endif
}

std::vector<std::shared_ptr<IDataPersistance>> DataPersistanceManager::FindAllDataPersistanceObjects() {
    std::vector<std::shared_ptr<IDataPersistance>> objects;

    for (const auto& object : GloomEngine::GetInstance()->gameObjects) {
        for (const auto& component : object.second->components) {
            if (std::dynamic_pointer_cast<IDataPersistance>(component.second) != nullptr) {
                objects.push_back(std::dynamic_pointer_cast<IDataPersistance>(component.second));
            }
        }
    }
    return objects;
}

