#pragma once

#include "light.h"

class DirectionalLight : public Light{

public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat aIntensity, GLfloat dIntensity,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir,
                     GLuint shadowWidth, GLuint shadowHeight);

    void use(GLfloat aIntensityLoc, GLfloat aColorLoc, 
             GLfloat dIntensityLoc, GLfloat DirectionLoc);

    glm::mat4 calculateLightTransform();

    ~DirectionalLight();

private:
    glm::vec3 m_Direction;

};