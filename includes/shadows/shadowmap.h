#pragma once

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap{

public:
    ShadowMap();

    virtual bool init(GLuint width, GLuint height);
    virtual void write();
    virtual void read(GLenum textureUnit);

    GLuint getShadowWidth() const {return m_ShadowWidth;}
    GLuint getShadowHeight() const {return m_ShadowHeight;}

    ~ShadowMap();

protected:
    GLuint m_FBO, m_ShadowMap;
    GLuint m_ShadowWidth, m_ShadowHeight;

};