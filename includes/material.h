#pragma once

#include <GL/glew.h>

class Material{

public:
    Material();
    Material(GLfloat sIntensity, GLfloat sShininess);

    void use(GLuint sIntensityLoc, GLuint sShininessLoc);

    ~Material();

private:
    GLfloat m_SpecularIntensity, m_Shininess;
};