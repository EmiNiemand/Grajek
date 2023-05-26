#include "Factories/PrefabFactory.h"

PrefabFactory::PrefabFactory() = default;

PrefabFactory::~PrefabFactory() {
    delete prefabFactory;
}

PrefabFactory* PrefabFactory::GetInstance() {
    if (prefabFactory == nullptr) {
        prefabFactory = new PrefabFactory();
    }
    return prefabFactory;
}