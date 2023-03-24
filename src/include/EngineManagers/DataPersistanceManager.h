//
// Created by szymo on 24/03/2023.
//

#ifndef GLOOMENGINE_DATAPERSISTANCEMANAGER_H
#define GLOOMENGINE_DATAPERSISTANCEMANAGER_H

#include <memory>
#include <vector>

class GloomEngine;
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

    void NewGame();
    void LoadGame();
    void SaveGame();

    DataPersistanceManager(DataPersistanceManager const&) = delete;
    DataPersistanceManager& operator=(DataPersistanceManager const&) = delete;
};


#endif //GLOOMENGINE_DATAPERSISTANCEMANAGER_H
