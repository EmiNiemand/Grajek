#ifndef IMAGE_CPP_OPPONENTPREFAB_H
#define IMAGE_CPP_OPPONENTPREFAB_H

#include "GameObjectsAndPrefabs/Prefab.h"

class OpponentPrefab : public Prefab {
public:
    OpponentPrefab(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~OpponentPrefab() override;

    std::shared_ptr<GameObject> Create() override;
};

#endif //IMAGE_CPP_OPPONENTPREFAB_H