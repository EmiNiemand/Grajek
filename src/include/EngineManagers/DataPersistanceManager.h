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
    static std::shared_ptr<GloomEngine> gloomEngine;

    std::shared_ptr<GameData> gameData;
    static std::shared_ptr<DataPersistanceManager> dataPersistanceManager;

private:
    DataPersistanceManager();
    std::vector<std::shared_ptr<IDataPersistance>> FindAllDataPersistanceObjects();

public:
    virtual ~DataPersistanceManager();

    static std::shared_ptr<DataPersistanceManager> GetInstance();

    //TODO: call this functions somewhere in the code
    void NewGame();
    void LoadGame(const std::string &dataDirectoryPath, const std::string &dataFileName);
    void SaveGame(const std::string &dataDirectoryPath, const std::string &dataFileName);

    DataPersistanceManager(DataPersistanceManager const&) = delete;
    DataPersistanceManager& operator=(DataPersistanceManager const&) = delete;
};


#endif //GLOOMENGINE_DATAPERSISTANCEMANAGER_H
