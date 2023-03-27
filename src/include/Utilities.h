#ifndef GLOOMENGINE_UTILITIES_H
#define GLOOMENGINE_UTILITIES_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>

class Utilities {
private:
    static Utilities* utilities;

public:
    Utilities(Utilities &other) = delete;
    void operator=(const Utilities&) = delete;

    static Utilities* GetInstance();

    glm::vec3 Lerp(glm::vec3 firstPoint, glm::vec3 secondPoint, float parameter);

private:
    Utilities();
    virtual ~Utilities();
};


#endif //GLOOMENGINE_UTILITIES_H
