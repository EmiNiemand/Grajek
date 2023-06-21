//
// Created by szymo on 21/06/2023.
//

#ifndef GLOOMENGINE_DEFAULT_H
#define GLOOMENGINE_DEFAULT_H

#include "GameObjectsAndPrefabs/Prefab.h"

namespace Crowd {

    class Default : public Prefab {
    public:
        Default(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag);
        ~Default() override;

        std::shared_ptr<GameObject> Create() override;
    };

} // Crowd

#endif //GLOOMENGINE_DEFAULT_H
