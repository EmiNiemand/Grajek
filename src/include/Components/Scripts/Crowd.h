//
// Created by szymo on 21/06/2023.
//

#ifndef GLOOMENGINE_CROWD_H
#define GLOOMENGINE_CROWD_H

#include "Components/Component.h"

namespace Crowd {
    class Crowd : public Component {
    private:
        std::shared_ptr<GameObject> jazzCrowd;
        std::shared_ptr<GameObject> drumCrowd;
    public:
        Crowd(const std::shared_ptr<GameObject> &parent, int id);
        ~Crowd() override;

        void Start() override;
        void OnDestroy() override;

        void OnEnemyDefeat(PlayerBadges badge);
    };
}



#endif //GLOOMENGINE_CROWD_H
