#ifndef GLOOMENGINE_BUTTON_H
#define GLOOMENGINE_BUTTON_H

#include "Components/Component.h"
#include "Components/UI/Text.h"
#include "LowLevelClasses/Mesh.h"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

class Button : public Component{
private:
    unsigned int textureID;
    unsigned int textureIsActive;
    std::shared_ptr<Mesh> textureMesh;
    std::shared_ptr<Mesh> textMesh;
    std::string text;
    glm::vec3 color;
    GLfloat x, y;
    std::map<GLchar, Character> Characters;

public:
    bool isActive = false;

public:
    Button(const std::shared_ptr<GameObject> &parent, int id);

    std::shared_ptr<Mesh> CreateMesh(float x, float y, float width, float height);
    /**
    * x from 0 to 1920\n
    * y from 0 to 1080
    */
    void LoadTexture(float x, float y, const std::string& path, const std::string& pathIsActive);
    /**
    * x from 0 to 1920\n
    * y from 0 to 1080
    */
    void LoadFont(std::string text, float x, float y, FT_UInt fontSize, glm::vec3 color, const std::string& path);

    void Update() override;

    void Draw();
};


#endif //GLOOMENGINE_BUTTON_H