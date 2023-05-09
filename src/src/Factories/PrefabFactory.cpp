#include "Factories/PrefabFactory.h"

PrefabFactory::PrefabFactory() = default;

PrefabFactory* PrefabFactory::GetInstance() {
    if (prefabFactory == nullptr) {
        prefabFactory = new PrefabFactory();
    }
    return prefabFactory;
}