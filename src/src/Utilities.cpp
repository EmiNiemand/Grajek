#include "Utilities.h"

Utilities* Utilities::utilities = nullptr;

Utilities::Utilities() = default;

Utilities::~Utilities() {}

glm::vec3 Utilities::Lerp(glm::vec3 firstPoint, glm::vec3 secondPoint, float parameter) {
    return firstPoint + (secondPoint - firstPoint) * parameter;
}

float Utilities::Lerp(float firstValue, float secondValue, float parameter) {
	return firstValue + (secondValue - firstValue) * parameter;
}

Utilities *Utilities::GetInstance() {
    if (utilities == nullptr) {
        utilities = new Utilities();
    }
    return utilities;
}

int Utilities::Hash(const std::string& text) {
    std::hash<std::string> hash;
    return (int)hash(text);
}
