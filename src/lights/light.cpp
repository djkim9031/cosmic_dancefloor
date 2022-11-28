#include "lights/light.h"

Light::Light(){

    m_Color = glm::vec3(1.f, 1.f, 1.f);
    m_AmbientIntensity = 1.f;
    m_DiffuseIntensity = 0.f;

}

Light::Light(GLfloat red, GLfloat green, GLfloat blue,
             GLfloat aIntensity, GLfloat dIntensity,
             GLuint shadowWidth, GLuint shadowHeight){

    m_Color = glm::vec3(red, green, blue);
    m_AmbientIntensity = aIntensity;
    m_DiffuseIntensity = dIntensity;

    m_ShadowMap = new ShadowMap();
    m_ShadowMap->init(shadowWidth, shadowHeight);

}

Light::~Light(){

}