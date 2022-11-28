#pragma once

#include <GL/glew.h>
#include "stb_init.h"

enum TEXTURE_DATA_DEPTH_TYPE{
        TEXTURE_DEPTH_RGB,
        TEXTURE_DEPTH_RGBA
};

class Texture{

public:
    Texture();
    Texture(const char* texFile);

    bool load_RGB();
    bool load_RGBA();

    void use();
    void clear();

    ~Texture();

private:
    GLuint m_TextureID;
    int m_Width, m_Height, m_BitDepth;
    const char* m_FileLocation;

    

};