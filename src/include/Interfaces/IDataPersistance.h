//
// Created by szymo on 24/03/2023.
//

#ifndef GLOOMENGINE_IDATAPERSISTANCE_H
#define GLOOMENGINE_IDATAPERSISTANCE_H

#include <memory>

class GameData;

//TODO: Implement this in player script where DataGame variables are implemented (money, reputation and set position)
// in LoadData https://www.youtube.com/watch?v=aUi9aijvpgs&t=903s
class IDataPersistance {
public:
    virtual void LoadData(std::shared_ptr<GameData> data) = 0;
    virtual void SaveData(std::shared_ptr<GameData> &data) = 0;
};


#endif //GLOOMENGINE_IDATAPERSISTANCE_H
