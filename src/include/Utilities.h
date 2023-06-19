#ifndef GLOOMENGINE_UTILITIES_H
#define GLOOMENGINE_UTILITIES_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>

class GameFont {
public:
    inline static const std::string Eggnog="Eggnog.ttf";
	inline static const std::string KanitLight="Kanit-Light.ttf";
    inline static const std::string KanitMedium="Kanit-Medium.ttf";
    inline static const std::string MarckScript="MarckScript.ttf";
};

class Color {
public:
    inline static const glm::vec3 Black = glm::vec3(0);
    inline static const glm::vec3 White = glm::vec3(1);
    inline static const glm::vec3 Red = glm::vec3(1, 0, 0);
    inline static const glm::vec3 Green = glm::vec3(0, 1, 0);
    inline static const glm::vec3 Blue = glm::vec3(0, 0, 1);
    inline static const glm::vec3 Yellow = glm::vec3(1, 1, 0);
    inline static const glm::vec3 Cyan = glm::vec3(0, 1, 1);
};

class Utilities {
private:
    inline static Utilities* utilities = nullptr;

public:
    Utilities(Utilities &other) = delete;
    void operator=(const Utilities&) = delete;

    static Utilities* GetInstance();

    static glm::vec3 Lerp(glm::vec3 firstPoint, glm::vec3 secondPoint, float parameter);
	static float Lerp(float firstValue, float secondValue, float parameter);
    static int Hash(const std::string& text);

private:
    Utilities();
    virtual ~Utilities();
};


#endif //GLOOMENGINE_UTILITIES_H
