#include "shadows/shadowmap.h"

ShadowMap::ShadowMap(){

    m_FBO = 0;
    m_ShadowMap = 0;
    m_ShadowWidth = 0;
    m_ShadowHeight = 0;

}

bool ShadowMap::init(GLuint width, GLuint height){

    m_ShadowWidth = width;
    m_ShadowHeight = height;

    glGenFramebuffers(1, &m_FBO);
    glGenTextures(1, &m_ShadowMap);
    glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ShadowWidth, m_ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.f, 1.f, 1.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE){
        printf("[ERROR]\t\t Framebuffer: %i\n", status);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void ShadowMap::write(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void ShadowMap::read(GLenum textureUnit){
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
}

ShadowMap::~ShadowMap(){
    
    if(m_FBO){
        glDeleteFramebuffers(1, &m_FBO);
    }
    if(m_ShadowMap){
        glDeleteTextures(1, &m_ShadowMap);
    }

}