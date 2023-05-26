#include "Utilities.h"

Utilities::Utilities() = default;

Utilities::~Utilities() {
    delete utilities;
}

Utilities *Utilities::GetInstance() {
    if (utilities == nullptr) {
        utilities = new Utilities();
    }
    return utilities;
}

glm::vec3 Utilities::Lerp(glm::vec3 firstPoint, glm::vec3 secondPoint, float parameter) {
    return firstPoint + (secondPoint - firstPoint) * parameter;
}

float Utilities::Lerp(float firstValue, float secondValue, float parameter) {
	return firstValue + (secondValue - firstValue) * parameter;
}

int Utilities::Hash(const std::string& text) {
    std::hash<std::string> hash;
    return (int)hash(text);
}
