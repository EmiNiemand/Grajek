//
// Created by szymo on 24/03/2023.
//

#ifndef GLOOMENGINE_FILEDATAHANDLER_H
#define GLOOMENGINE_FILEDATAHANDLER_H

#include <string>
#include "nlohmann/json.hpp"
#include "StaticObjData.h"

class GameData;

class FileDataHandler {
private:
    std::string dataDirectoryPath;
    std::string dataFileName;

public:
    FileDataHandler(std::string dataDirectoryPath, std::string dataFileName);
    virtual ~FileDataHandler();

    std::shared_ptr<GameData> LoadGame();
    void SaveGame(std::shared_ptr<GameData> gameData);

    std::vector<std::shared_ptr<StaticObjData>> LoadMap();
    void SaveMap(std::vector<std::shared_ptr<StaticObjData>> mapData);

private:
    void to_json(nlohmann::json &json, const std::shared_ptr<GameData>& gameData);
    void from_json(const nlohmann::json &json, const std::shared_ptr<GameData>& gameData);

    void to_json(nlohmann::json &json, std::vector<std::shared_ptr<StaticObjData>> mapData);
    void from_json(const nlohmann::json &json, std::vector<std::shared_ptr<StaticObjData>> mapData);
};


#endif //GLOOMENGINE_FILEDATAHANDLER_H
