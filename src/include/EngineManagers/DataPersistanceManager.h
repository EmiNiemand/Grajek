//
// Created by szymo on 24/03/2023.
//

#ifndef GLOOMENGINE_DATAPERSISTANCEMANAGER_H
#define GLOOMENGINE_DATAPERSISTANCEMANAGER_H

#include <memory>
#include <vector>
#include <string>

class GloomEngine;
class FileDataHandler;
class GameData;
class IDataPersistance;

class DataPersistanceManager {
private:
    std::shared_ptr<GameData> gameData;
    inline static DataPersistanceManager* dataPersistanceManager;

private:
    DataPersistanceManager();
    static std::vector<std::shared_ptr<IDataPersistance>> FindAllDataPersistanceObjects();

public:
    DataPersistanceManager(DataPersistanceManager &other) = delete;
    void operator=(const DataPersistanceManager&) = delete;
    virtual ~DataPersistanceManager();

    static DataPersistanceManager* GetInstance();

    void NewGame();
    void LoadGame(const std::string &dataDirectoryPath, const std::string &dataFileName);
    void SaveGame(const std::string &dataDirectoryPath, const std::string &dataFileName);
};


#endif //GLOOMENGINE_DATAPERSISTANCEMANAGER_H
