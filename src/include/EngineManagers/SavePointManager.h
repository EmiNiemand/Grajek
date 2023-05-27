#ifndef GAME_CPP_SAVEPOINTMANAGER_H
#define GAME_CPP_SAVEPOINTMANAGER_H

#include <memory>
#include <unordered_map>

class SavePointTrigger;
class Image;

class SavePointManager {
private:
    inline static SavePointManager* savePointManager;

public:
    std::shared_ptr<Image> buttonImage;
    std::shared_ptr<SavePointTrigger> activeSavePoint;
    std::unordered_map<int, std::shared_ptr<SavePointTrigger>> savePoints;

private:
    explicit SavePointManager();

public:
    SavePointManager(SavePointManager &other) = delete;
    void operator=(const SavePointManager&) = delete;
    virtual ~SavePointManager();

    static SavePointManager* GetInstance();

    void NotifyMenuIsActive();
    void NotifyMenuIsNotActive();
};

#endif //GAME_CPP_SAVEPOINTMANAGER_H