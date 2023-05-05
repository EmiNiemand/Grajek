//
// Created by masterktos on 01.04.23.
//

#ifndef GLOOMENGINE_PREFAB_H
#define GLOOMENGINE_PREFAB_H

#include "GameObject.h"
#include "Factories/PrefabFactory.h"

class Prefab : public GameObject {
protected:
    std::string prefabName;

public:
    Prefab(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);

    template<class T>
    static std::shared_ptr<GameObject> Instantiate(std::string objectName = typeid(T).name()) {
        return PrefabFactory::GetInstance()->CreateGameObjectFromPrefab<T>(objectName);
    }

    const std::string& GetPrefabName() const;
    virtual std::shared_ptr<GameObject> Create() = 0;
};


#endif //GLOOMENGINE_PREFAB_H
