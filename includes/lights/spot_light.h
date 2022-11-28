#pragma once
#include "point_light.h"

class SpotLight : public PointLight{

public:
    SpotLight();
    SpotLight(GLfloat red, GLfloat green, GLfloat blue,
              GLfloat aIntensity, GLfloat dIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat con, GLfloat lin, GLfloat exp, GLfloat FOVAngle,
              GLuint shadowWidth, GLuint shadowHeight,
              GLfloat nearPlane, GLfloat farPlane);

    void use(GLfloat aIntensityLoc, GLfloat aColorLoc, 
             GLfloat dIntensityLoc, GLfloat positionLoc, GLfloat directionLoc,
             GLfloat constantLoc, GLfloat linearLoc, GLfloat exponentLoc, GLfloat FOVAngleLoc);

    void setFlash(glm::vec3 pos, glm::vec3 dir);

    ~SpotLight();

private:
    glm::vec3 m_Direction;
    GLfloat m_FOVAngle, m_CosFOVAngle;
    
};