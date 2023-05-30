#ifndef GLOOMENGINE_MENU_H
#define GLOOMENGINE_MENU_H

#include "Components/Component.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

class Button;
class Text;
class Image;

class Menu : public Component {
public:
    std::shared_ptr<Button> activeButton;

public:
    Menu(const std::shared_ptr<GameObject> &parent, int id);
    ~Menu() override;

    void OnDestroy() override;

    virtual void HideMenu();
    inline virtual void ChangeActiveButton(glm::vec2 moveVector);
    std::shared_ptr<Image> AddImage(std::string name, int x, int y, const std::string &path, float z = 0.0f);
    std::shared_ptr<Text> AddText(std::string name, std::string text, int x, int y, FT_UInt fontSize = 18, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f), const std::string& path = "Kanit-Light.ttf");
    std::shared_ptr<Button> AddButton(std::string name, int x, int y, const std::string& pathInactive, const std::string& pathActive, std::string text = "", FT_UInt fontSize = 18, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f), const std::string &fontPath = "Kanit-Light.ttf", float z = 0.0f);
    inline virtual void OnClick(){};
};

#endif //GLOOMENGINE_MENU_H
