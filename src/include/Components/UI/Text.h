#ifndef GLOOMENGINE_TEXT_H
#define GLOOMENGINE_TEXT_H

#include <map>
#include <string>
#include "Components/UI/UIComponent.h"
#include "LowLevelClasses/Mesh.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

class Text : public UIComponent {
private:
    std::shared_ptr<Mesh> mesh;
    std::string text;
    glm::vec3 color;
    GLfloat x, y;
    std::map<GLchar, Character> Characters;

public:
    Text(const std::shared_ptr<GameObject> &parent, int id);

    void CreateMesh(float x, float y);
    /**
    * x from 0 to 1920\n
    * y from 0 to 1080
    */
    void LoadFont(std::string text, float x, float y, FT_UInt fontSize, glm::vec3 color, const std::string& path);

    void Update() override;

    void Draw() override;
};

#endif //GLOOMENGINE_TEXT_H
