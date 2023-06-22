//
// Created by szymo on 21/06/2023.
//

#ifndef GLOOMENGINE_JAZZ_H
#define GLOOMENGINE_JAZZ_H

#include "GameObjectsAndPrefabs/Prefab.h"

namespace Crowd {

    class Jazz : public Prefab {
    public:
        Jazz(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag);
        ~Jazz() override;

        std::shared_ptr<GameObject> Create() override;
    };

} // Crowd

#endif //GLOOMENGINE_JAZZ_H
