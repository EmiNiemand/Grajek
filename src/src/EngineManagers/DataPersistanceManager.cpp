//
// Created by szymo on 24/03/2023.
//

#include "EngineManagers/DataPersistanceManager.h"
#include "GloomEngine.h"
#include "LowLevelClasses/GameData.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Component.h"
#include "Interfaces/IDataPersistance.h"

std::shared_ptr<GloomEngine> DataPersistanceManager::gloomEngine = nullptr;
std::shared_ptr<DataPersistanceManager> DataPersistanceManager::dataPersistanceManager = nullptr;

DataPersistanceManager::DataPersistanceManager() {}

DataPersistanceManager::~DataPersistanceManager() {}

std::shared_ptr<DataPersistanceManager> DataPersistanceManager::GetInstance() {
    if (dataPersistanceManager == nullptr) {
        dataPersistanceManager = std::shared_ptr<DataPersistanceManager>();
    }
    return dataPersistanceManager;
}

void DataPersistanceManager::NewGame() {
    gameData = std::make_unique<GameData>();
}

void DataPersistanceManager::LoadGame() {
    if (gameData == nullptr) {
        NewGame();
    }
    std::vector<std::shared_ptr<IDataPersistance>> dataPersistanceObjects = FindAllDataPersistanceObjects();

    for (const auto& object : dataPersistanceObjects) {
        object->LoadData(gameData);
    }
}

void DataPersistanceManager::SaveGame() {
    std::vector<std::shared_ptr<IDataPersistance>> dataPersistanceObjects = FindAllDataPersistanceObjects();

    for (const auto& object : dataPersistanceObjects) {
        object->SaveData(gameData);
    }
}

std::vector<std::shared_ptr<IDataPersistance>> DataPersistanceManager::FindAllDataPersistanceObjects() {
    std::vector<std::shared_ptr<IDataPersistance>> objects;

    for (const auto& object : gloomEngine->gameObjects) {
        for (const auto& component : object.second->components) {
            if (std::dynamic_pointer_cast<IDataPersistance>(component.second) != nullptr) {
                objects.push_back(std::dynamic_pointer_cast<IDataPersistance>(component.second));
            }
        }
    }
    return objects;
}

