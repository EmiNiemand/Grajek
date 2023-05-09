#include "GameObjectsAndPrefabs/Prefab.h"

Prefab::Prefab(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        GameObject(name, id, parent, tag) {}


const std::string& Prefab::GetPrefabName() const {
    return prefabName;
}
