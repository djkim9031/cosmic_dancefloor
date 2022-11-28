#include "texture.h"

Texture::Texture(){

    m_TextureID = 0;
    m_Width = 0;
    m_Height = 0;
    m_BitDepth = 0;
    m_FileLocation = nullptr;

}

Texture::Texture(const char* texFile){

    m_TextureID = 0;
    m_Width = 0;
    m_Height = 0;
    m_BitDepth = 0;
    m_FileLocation = texFile;

}

bool Texture::load_RGB(){

    unsigned char* texData = stbi_load(m_FileLocation, &m_Width, &m_Height, &m_BitDepth, 0);
    if(!texData){
        printf("[ERROR-RGB]\t Texture file at %s cannot be loaded.\n", m_FileLocation);
        return false;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texData);

    return true;
}

bool Texture::load_RGBA(){

    unsigned char* texData = stbi_load(m_FileLocation, &m_Width, &m_Height, &m_BitDepth, 0);
    if(!texData){
        printf("[ERROR-RGBA]\t Texture file at %s cannot be loaded.\n", m_FileLocation);
        return false;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texData);

    return true;
}

void Texture::use(){

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

}

void Texture::clear(){

    glDeleteTextures(1, &m_TextureID);
    m_TextureID = 0;
    m_Width = 0;
    m_Height = 0;
    m_BitDepth = 0;
    m_FileLocation = nullptr;

}

Texture::~Texture(){
    clear();
}