#ifndef GLOOMENGINE_MAINMENUPREFAB_H
#define GLOOMENGINE_MAINMENUPREFAB_H

#include "GameObjectsAndPrefabs/Prefab.h"

class MainMenuPrefab : public Prefab {
public:
    MainMenuPrefab(const std::string &name, uint32_t id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~MainMenuPrefab() override;

    std::shared_ptr<GameObject> Create() override;
};

#endif //GLOOMENGINE_MAINMENUPREFAB_H