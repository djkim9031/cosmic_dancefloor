#include "lights/point_light.h"

PointLight::PointLight() : Light(){

    m_Position = glm::vec3(0.f, 0.f, 0.f);
    m_Constant = 1.f;
    m_Linear = 0.f;
    m_Exponent = 0.f;

}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat aIntensity, GLfloat dIntensity,
                       GLfloat xPos, GLfloat yPos, GLfloat zPos,
                       GLfloat con, GLfloat lin, GLfloat exp,
                       GLuint shadowWidth, GLuint shadowHeight,
                       GLfloat nearPlane, GLfloat farPlane)
                       : Light (red, green, blue, aIntensity, dIntensity, shadowWidth, shadowHeight){

    m_Position = glm::vec3(xPos, yPos, zPos);
    m_Constant = con;
    m_Linear = lin;
    m_Exponent = exp;
    m_FarPlane = farPlane;

    float aspect =(float)shadowWidth/(float)shadowHeight;
    m_LightProj = glm::perspective(glm::radians(90.f), aspect, nearPlane, farPlane);

    m_ShadowMap = new OmniShadowMap();
    m_ShadowMap->init(shadowWidth, shadowHeight);

}

void PointLight::use(GLfloat aIntensityLoc, GLfloat aColorLoc, 
                     GLfloat dIntensityLoc, GLfloat positionLoc,
                     GLfloat constantLoc, GLfloat linearLoc, GLfloat exponentLoc){

    glUniform1f(aIntensityLoc, m_AmbientIntensity);
    glUniform3f(aColorLoc, m_Color.x, m_Color.y, m_Color.z);
    glUniform1f(dIntensityLoc, m_DiffuseIntensity);
    glUniform3f(positionLoc, m_Position.x, m_Position.y, m_Position.z);

    glUniform1f(constantLoc, m_Constant);
    glUniform1f(linearLoc, m_Linear);
    glUniform1f(exponentLoc, m_Exponent);

}

std::vector<glm::mat4> PointLight::calculateLightTransform(){

    std::vector<glm::mat4> lightMatrices;
    
    //Should strictly have the order in which cube texture is created (+x, -x, +y, -y, +z, -z)
    //For +-x and +-z, the up vector should be +y originally, but because in this code, inverted normal is used for lighting/shadow calculation,
    //up vector should be inverted as -y
    lightMatrices.push_back(m_LightProj * glm::lookAt(m_Position, m_Position + glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)));
    lightMatrices.push_back(m_LightProj * glm::lookAt(m_Position, m_Position + glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)));
    lightMatrices.push_back(m_LightProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)));
    lightMatrices.push_back(m_LightProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f)));
    lightMatrices.push_back(m_LightProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f)));
    lightMatrices.push_back(m_LightProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f)));

    return lightMatrices;
}

PointLight::~PointLight(){

}