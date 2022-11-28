#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H 

class Text{

public:
    Text(std::string fontDir, const GLint win_width, const GLint win_height);

    void init();
    void render(Shader *shader, std::string text, float x, float y, float scale, glm::vec3 color);
    void clear();

    ~Text();

private:
    GLint m_WINDOW_WIDTH;
    GLint m_WINDOW_HEIGHT;
    GLuint m_VAO, m_VBO;
    GLuint m_TextureID;
    std::string m_Font;

    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    std::map<GLchar, Character> m_Characters;

};