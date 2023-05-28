#include "Factories/ComponentFactory.h"

ComponentFactory::ComponentFactory() = default;

ComponentFactory* ComponentFactory::GetInstance() {
    if (componentFactory == nullptr) {
        componentFactory = new ComponentFactory();
    }
    return componentFactory;
}

ComponentFactory::~ComponentFactory() {
    delete componentFactory;
}
