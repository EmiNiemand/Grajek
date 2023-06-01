#ifndef GLOOMENGINE_BUTTON_H
#define GLOOMENGINE_BUTTON_H

#include "Components/UI/UIComponent.h"
#include "Components/UI/Text.h"
#include "LowLevelClasses/Mesh.h"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

class Button : public UIComponent{
private:
    unsigned int textureID;
    unsigned int textureIsActive;
    std::shared_ptr<Mesh> textureMesh;
    std::shared_ptr<Mesh> textMesh;
    int width, height;
    int textX ,textY;
    glm::vec3 color;
    FT_UInt fontSize;
    std::map<GLchar, Character> Characters;
    float z = 0.0f;

public:
    int x, y;
    std::string text;
    bool isActive = false;
    std::shared_ptr<Button> previousButton, nextButton;

public:
    Button(const std::shared_ptr<GameObject> &parent, int id);

    std::shared_ptr<Mesh> CreateMesh(int x, int y, int width, int height);
    /**
    * x from 0 to 1920\n
    * y from 0 to 1080
    */
    void LoadTexture(int x, int y, const std::string& path, const std::string& pathIsActive, float z = 0.0f);
    /**
    * x from 0 to 1920\n
    * y from 0 to 1080
    */
    void LoadFont(std::string text, FT_UInt fontSize = 18, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f), const std::string &path = "Kanit-Light.ttf");
    void ChangeText(std::string newText);
    void ChangePosition(int newX, int newY);

	int GetWidth();
	int GetHeight();

    void Update() override;
    void OnDestroy() override;

    void Draw() override;
};


#endif //GLOOMENGINE_BUTTON_H