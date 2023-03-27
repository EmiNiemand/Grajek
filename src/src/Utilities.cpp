#include "Utilities.h"

Utilities* Utilities::utilities = nullptr;

Utilities::Utilities() {}

Utilities::~Utilities() {}

glm::vec3 Utilities::Lerp(glm::vec3 firstPoint, glm::vec3 secondPoint, float parameter) {
    return firstPoint + (secondPoint - firstPoint) * parameter;
}

Utilities *Utilities::GetInstance() {
    if (utilities == nullptr) {
        utilities = new Utilities();
    }
    return utilities;
}
