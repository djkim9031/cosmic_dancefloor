#include <lights/directional_light.h>

DirectionalLight::DirectionalLight() : Light(){

    m_Direction = glm::vec3(0.f, -1.f, 0.f);
    m_LightProj = glm::ortho(-20.f, 20.f, -20.f, 20.f, 0.1f, 100.f);

}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat aIntensity, GLfloat dIntensity,
                                   GLfloat xDir, GLfloat yDir, GLfloat zDir,
                                   GLuint shadowWidth, GLuint shadowHeight)
                                   : Light(red, green, blue, aIntensity, dIntensity, shadowWidth, shadowHeight){

    m_Direction = glm::vec3(xDir, yDir, zDir);
    m_LightProj = glm::ortho(-20.f, 20.f, -20.f, 20.f, 0.1f, 100.f);

}

void DirectionalLight::use(GLfloat aIntensityLoc, GLfloat aColorLoc, 
                           GLfloat dIntensityLoc, GLfloat DirectionLoc){

    glUniform1f(aIntensityLoc, m_AmbientIntensity);
    glUniform3f(aColorLoc, m_Color.x, m_Color.y, m_Color.z);
    glUniform1f(dIntensityLoc, m_DiffuseIntensity);
    glUniform3f(DirectionLoc, m_Direction.x, m_Direction.y, m_Direction.z);

}

glm::mat4 DirectionalLight::calculateLightTransform(){

    glm::vec3 front = glm::normalize(m_Direction);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    //3d world coord to 3d directionalLight coord, and orthogonal projection
    return m_LightProj * glm::lookAt(-m_Direction, glm::vec3(0.f, 0.f, 0.f), up);

}

DirectionalLight::~DirectionalLight(){
    
}