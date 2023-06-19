#include <iostream>
#include "Components/UI/Button.h"
#include "LowLevelClasses/Mesh.h"
#include "EngineManagers/UIManager.h"
#include "stb_image.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

#define BASE_PATH_FONT "res/fonts/"
#define BASE_PATH_TEXTURE "res/textures/"

Button::Button(const std::shared_ptr<GameObject> &parent, int id) : UIComponent(parent, id) {}

std::shared_ptr<Mesh> Button::CreateMesh(int x, int y, int width, int height) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = {0, 1, 3,
                                         0, 2, 3};
    std::vector<Texture> textures;

    Vertex vertex1, vertex2, vertex3, vertex4;
    // left bottom
    vertex1.position = glm::vec3((float)x/960-1, (float)y/540-1, z);
    vertex1.texCoords = glm::vec2(0.0f, 0.0f);
    // left top
    vertex2.position = glm::vec3((float)x/960-1, (float)y/540-1 + (float)height/540, z);
    vertex2.texCoords = glm::vec2(0.0f, 1.0f);
    // right bottom
    vertex3.position = glm::vec3((float)x/960-1 + (float)width/960, (float)y/540-1, z);
    vertex3.texCoords = glm::vec2(1.0f, 0.0f);
    // right top
    vertex4.position = glm::vec3((float)x/960-1 + (float)width/960, (float)y/540-1 + (float)height/540, z);
    vertex4.texCoords = glm::vec2(1.0f, 1.0f);

    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    vertices.push_back(vertex3);
    vertices.push_back(vertex4);

    return std::make_shared<Mesh>(vertices, indices, textures);
}

void Button::LoadTexture(int x, int y, const std::string& path, const std::string& pathIsActive, float z) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int width, height, nrChannels;
    std::string file = BASE_PATH_TEXTURE + path;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        this->z = z;
        this->textureMesh = CreateMesh(x, y, width, height);
        this->width = width;
        this->height = height;
        this->x = x;
        this->y = y;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glGenTextures(1, &textureIsActive);
    glBindTexture(GL_TEXTURE_2D, textureIsActive);
    file = BASE_PATH_TEXTURE + pathIsActive;
    data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureIsActive);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    UIManager::GetInstance()->shader->Activate();
    UIManager::GetInstance()->shader->SetInt("texture1", 0);
    UIManager::GetInstance()->shader->SetInt("texture2", 1);
}

void Button::LoadFont(std::string text, FT_UInt fontSize, glm::vec3 color, const std::string &path) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    FT_Face face;
    std::string file = BASE_PATH_FONT + path;
    if (FT_New_Face(ft, file.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    this->textMesh = CreateMesh((int)(this->textureMesh->vertices[0].position.x*960.0f)+960, (int)(this->textureMesh->vertices[0].position.y*960.0f)+960, 0, 0);
    this->text = std::move(text);
    this->color = color;
    this->fontSize = fontSize;
    this->textX = (int)(this->textureMesh->vertices[0].position.x*960.0f)+960;
    this->textY = (int)(this->textureMesh->vertices[0].position.y*540.0f)+540 + height / 2 - fontSize / 3;

    int lengthX = 0;
    for (std::string::const_iterator c = this->text.begin(); c != this->text.end(); c++) {
        lengthX += (Characters[*c].Advance >> 6);
    }
    textX += (width - lengthX) / 2;
}

void Button::ChangeText(std::string newText) {
    this->text = std::move(newText);
    this->textX = (int)(this->textureMesh->vertices[0].position.x * 960.0f) + 960;
    this->textY = (int)(this->textureMesh->vertices[0].position.y * 540.0f) + 540 + height / 2 - fontSize / 3;

    int lengthX = 0;
    for (std::string::const_iterator c = this->text.begin(); c != this->text.end(); c++) {
        lengthX += (Characters[*c].Advance >> 6);
    }
    textX += (width - lengthX) / 2;
}

void Button::ChangePosition(int newX, int newY) {
    textureMesh = CreateMesh(newX, newY, width, height);
    ChangeText(text);
}

void Button::ChangeZ(float newZ) {
    z = newZ;

    textMesh->vertices[0].position.z = z-0.01f;
    textMesh->vertices[1].position.z = z-0.01f;
    textMesh->vertices[2].position.z = z-0.01f;
    textMesh->vertices[3].position.z = z-0.01f;

    textureMesh = CreateMesh(x, y, width,height);
}

void Button::Update() {
#ifdef DEBUG
    ZoneScopedNC("Button", 0x800080);
#endif
    UIComponent::Update();
}

void Button::Draw() {
    // Render text
    if (!this->text.empty()) {
        UIManager::GetInstance()->shader->Activate();
        UIManager::GetInstance()->shader->SetBool("isText", true);
        UIManager::GetInstance()->shader->SetVec3("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(textMesh->vao);

        // Iterate through all characters
        GLfloat x2 = (float)textX;
        GLfloat y2 = (float)textY;

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            Character ch = Characters[*c];

            GLfloat xpos = x2 + ch.Bearing.x;
            GLfloat ypos = y2 - (ch.Size.y - ch.Bearing.y);

            GLfloat w = ch.Size.x;
            GLfloat h = ch.Size.y;
            // Update VBO for each character
            this->textMesh->vertices[0].position.x = xpos / 960 - 1;
            this->textMesh->vertices[0].position.y = (ypos + h) / 540 - 1;
            this->textMesh->vertices[1].position.x = xpos / 960 - 1;
            this->textMesh->vertices[1].position.y = ypos / 540 - 1;
            this->textMesh->vertices[2].position.x = (xpos + w) / 960 - 1;
            this->textMesh->vertices[2].position.y = (ypos + h) / 540 - 1;
            this->textMesh->vertices[3].position.x = (xpos + w) / 960 - 1;
            this->textMesh->vertices[3].position.y = ypos / 540 - 1;

            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, this->textMesh->GetVBO());
            glBufferSubData(GL_ARRAY_BUFFER, 0, this->textMesh->vertices.size() * sizeof(Vertex),
                            &this->textMesh->vertices[0]); // Be sure to use glBufferSubData and not glBufferData
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x2 += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Render texture
    UIManager::GetInstance()->shader->Activate();
    UIManager::GetInstance()->shader->SetBool("isText", false);
    glActiveTexture(GL_TEXTURE0);
	UIManager::GetInstance()->shader->SetInt("texture1", 0);
    if(isActive) {
        glBindTexture(GL_TEXTURE_2D, textureIsActive);
    } else {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    glBindVertexArray(textureMesh->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

int Button::GetWidth() {
	return width;
}

int Button::GetHeight() {
	return height;
}

void Button::OnDestroy() {
    glDeleteTextures(1, &textureID);
    for (auto&& character : Characters) {
        glDeleteTextures(1, &character.second.TextureID);
    }
    textureMesh.reset();
    textMesh.reset();
    previousButton.reset();
    nextButton.reset();
    Component::OnDestroy();
}
