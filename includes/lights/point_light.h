#pragma once

#include <vector>
#include "light.h"
#include "../shadows/omni_shadowmap.h"

class PointLight : public Light{

public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue,
               GLfloat aIntensity, GLfloat dIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp,
               GLuint shadowWidth, GLuint shadowHeight,
               GLfloat nearPlane, GLfloat farPlane);

    void use(GLfloat aIntensityLoc, GLfloat aColorLoc, 
             GLfloat dIntensityLoc, GLfloat positionLoc,
             GLfloat constantLoc, GLfloat linearLoc, GLfloat exponentLoc);

    std::vector<glm::mat4> calculateLightTransform();
    GLfloat getFarPlane() const {return m_FarPlane;}
    glm::vec3 getPosition() const {return m_Position;}

    ~PointLight();

protected:
    glm::vec3 m_Position;
    GLfloat m_Constant, m_Linear, m_Exponent;
    GLfloat m_FarPlane;
};