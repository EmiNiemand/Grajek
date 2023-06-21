#ifndef GLOOMENGINE_BUTTON_H
#define GLOOMENGINE_BUTTON_H

#include "Components/UI/UIComponent.h"
#include "Components/UI/Text.h"
#include "LowLevelClasses/Mesh.h"
#include <map>
#include <ft2build.h>
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include FT_FREETYPE_H

class Button : public UIComponent{
private:
    unsigned int textureID;
    unsigned int textureIsActive;
    std::shared_ptr<Mesh> textureMesh;
    std::shared_ptr<Mesh> textMesh;
    glm::vec2 leftBottom{}, leftTop{}, rightBottom{}, rightTop{};
    int width, height;
    int textX, textY;
    glm::vec3 color = glm::vec3(1);
    glm::vec3 textColor;
    FT_UInt fontSize;
    std::map<GLchar, Character> Characters;
    float z = 0.0f;
    glm::vec2 scale = glm::vec2(1);

public:
    int x, y;
    std::string text;
    bool isActive = false;
    std::shared_ptr<Button> left, right, up, down;

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
    void ChangeZ(float newZ);

    void SetScale(float scale) override;
    void SetColor(glm::vec3 newColor) override;

	int GetWidth();
	int GetHeight();

    void Update() override;
    void OnDestroy() override;

    void Draw() override;
};


#endif //GLOOMENGINE_BUTTON_H