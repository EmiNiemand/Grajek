#include <iostream>
#include <utility>
#include "Components/UI/Text.h"
#include "EngineManagers/UIManager.h"

#define BASE_PATH "res/fonts/"

Text::Text(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void Text::CreateMesh(float x, float y) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = {0, 1, 3,
                                         0, 2, 3};
    std::vector<Texture> textures;

    Vertex vertex1, vertex2, vertex3, vertex4;
    // left bottom
    vertex1.position = glm::vec3(0.0f);
    vertex1.texCoords = glm::vec2(0.0f, 0.0f);
    // left top
    vertex2.position = glm::vec3( 0.0f);
    vertex2.texCoords = glm::vec2(0.0f, 1.0f);
    // right bottom
    vertex3.position = glm::vec3(0.0f);
    vertex3.texCoords = glm::vec2(1.0f, 0.0f);
    // right top
    vertex4.position = glm::vec3(0.0f);
    vertex4.texCoords = glm::vec2(1.0f, 1.0f);

    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    vertices.push_back(vertex3);
    vertices.push_back(vertex4);

    mesh = std::make_shared<Mesh>(vertices, indices, textures);
}

void Text::LoadFont(std::string text, float x, float y, FT_UInt fontSize, glm::vec3 color, const std::string& path) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    FT_Face face;
    std::string file = BASE_PATH + path;
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
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
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

    this->CreateMesh(x, y);
    this->text = std::move(text);
    this->x = x;
    this->y = y;
    this->color = color;
}

void Text::Update() {
    Draw();
    Component::Update();
}

void Text::Draw() {
    UIManager::GetInstance()->shader->Activate();
    UIManager::GetInstance()->shader->SetBool("isText", true);
    UIManager::GetInstance()->shader->SetVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(mesh->vao);

    // Iterate through all characters
    GLfloat x2 = this->x;
    GLfloat y2 = this->y;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x2 + ch.Bearing.x;
        GLfloat ypos = y2 - (ch.Size.y - ch.Bearing.y);

        GLfloat w = ch.Size.x;
        GLfloat h = ch.Size.y;
        // Update VBO for each character
        this->mesh->vertices[0].position.x = xpos/960-1;
        this->mesh->vertices[0].position.y = (ypos+h)/540-1;
        this->mesh->vertices[1].position.x = xpos/960-1;
        this->mesh->vertices[1].position.y = ypos/540-1;
        this->mesh->vertices[2].position.x = (xpos+w)/960-1;
        this->mesh->vertices[2].position.y = (ypos+h)/540-1;
        this->mesh->vertices[3].position.x = (xpos+w)/960-1;
        this->mesh->vertices[3].position.y = ypos/540-1;

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->mesh->GetVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, this->mesh->vertices.size() * sizeof(Vertex), &this->mesh->vertices[0]); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x2 += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}