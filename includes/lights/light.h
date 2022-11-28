#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../shadows/shadowmap.h"

class Light{

public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue,
          GLfloat aIntensity, GLfloat dIntensity,
          GLuint shadowWidth, GLuint shadowHeight);

    ShadowMap* getShadowMap() const {return m_ShadowMap;}

    ~Light();

protected:
    glm::vec3 m_Color;
    GLfloat m_AmbientIntensity;
    GLfloat m_DiffuseIntensity;

    glm::mat4 m_LightProj;
    ShadowMap* m_ShadowMap;
};