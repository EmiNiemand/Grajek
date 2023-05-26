#include "Factories/ComponentFactory.h"

ComponentFactory::ComponentFactory() {
    delete componentFactory;
}

ComponentFactory* ComponentFactory::GetInstance() {
    if (componentFactory == nullptr) {
        componentFactory = new ComponentFactory();
    }
    return componentFactory;
}
