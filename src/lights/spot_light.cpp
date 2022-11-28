#include "lights/spot_light.h"

SpotLight::SpotLight() : PointLight(){

    m_Direction = glm::normalize(glm::vec3(0.f, -1.f, 0.f));
    m_FOVAngle = 0.f;
    m_CosFOVAngle = cosf(glm::radians(m_FOVAngle));

}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat aIntensity, GLfloat dIntensity,
                     GLfloat xPos, GLfloat yPos, GLfloat zPos,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir,
                     GLfloat con, GLfloat lin, GLfloat exp, GLfloat FOVAngle,
                     GLuint shadowWidth, GLuint shadowHeight,
                     GLfloat nearPlane, GLfloat farPlane)
                     : PointLight(red, green, blue, aIntensity, dIntensity,
                                  xPos, yPos, zPos, con, lin, exp,
                                  shadowWidth, shadowHeight, nearPlane, farPlane){

    m_Direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
    m_FOVAngle = FOVAngle;
    m_CosFOVAngle = cosf(glm::radians(m_FOVAngle));

}

void SpotLight::use(GLfloat aIntensityLoc, GLfloat aColorLoc, 
                    GLfloat dIntensityLoc, GLfloat positionLoc, GLfloat directionLoc,
                    GLfloat constantLoc, GLfloat linearLoc, GLfloat exponentLoc, GLfloat FOVAngleLoc){

    glUniform1f(aIntensityLoc, m_AmbientIntensity);
    glUniform3f(aColorLoc, m_Color.x, m_Color.y, m_Color.z);
    glUniform1f(dIntensityLoc, m_DiffuseIntensity);
    glUniform3f(positionLoc, m_Position.x, m_Position.y, m_Position.z);

    glUniform1f(constantLoc, m_Constant);
    glUniform1f(linearLoc, m_Linear);
    glUniform1f(exponentLoc, m_Exponent);

    glUniform3f(directionLoc, m_Direction.x, m_Direction.y, m_Direction.z);
    glUniform1f(FOVAngleLoc, m_CosFOVAngle);

}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir){

    m_Position = pos;
    m_Direction = dir;

}

SpotLight::~SpotLight(){
    
}