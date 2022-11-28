#include "camera.h"

Camera::Camera(){

    m_Position = glm::vec3(0.f, 0.f, 0.f);
    m_WorldUp = glm::vec3(0.f, 1.f, 0.f);
    m_Pitch = 0.f;
    m_Yaw = 0.f;
    m_Roll = 0.f;
    m_Front = glm::vec3(0.f, 0.f, -1.f);

    m_TranslationalSpeed = 1.f;
    m_RotationalSpeed = 1.f;

}
    
Camera::Camera(glm::vec3 position, glm::vec3 up, 
               GLfloat pitch, GLfloat yaw, GLfloat roll, 
               GLfloat translationalSpeed, GLfloat rotationalSpeed){

    m_Position = position;
    m_WorldUp = up;
    m_Pitch = pitch;
    m_Yaw = yaw;
    m_Roll = roll;
    m_Front = glm::vec3(0.f, 0.f, -1.f);

    m_TranslationalSpeed = translationalSpeed;
    m_RotationalSpeed = rotationalSpeed;

    update();

}
    
void Camera::keyControl(bool *keys, GLfloat deltaTime){

    GLfloat velocity = m_TranslationalSpeed*deltaTime;
    if(keys[GLFW_KEY_W]){
        m_Position += m_Front*velocity;
    }
    if(keys[GLFW_KEY_S]){
        m_Position -= m_Front*velocity;
    }
    if(keys[GLFW_KEY_A]){
        m_Position -= m_Right*velocity;
    }
    if(keys[GLFW_KEY_D]){
        m_Position += m_Right*velocity;
    }
    if(keys[GLFW_KEY_COMMA]){
        m_Roll += 10*velocity;
        m_WorldUp.x = sin(glm::radians(m_Roll));
        m_WorldUp.y = cos(glm::radians(m_Roll));
        update();
    }
    if(keys[GLFW_KEY_PERIOD]){
        m_Roll -= 10*velocity;
        m_WorldUp.x = sin(glm::radians(m_Roll));
        m_WorldUp.y = cos(glm::radians(m_Roll));
        update();
    }

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange){

    xChange *= m_RotationalSpeed;
    yChange *= m_RotationalSpeed;

    m_Yaw -= xChange;
    m_Pitch -= yChange;

    update();

}

glm::mat4 Camera::calculateViewMatrix(){
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::vec3 Camera::getCameraPosition(){
    return m_Position;
}
   
glm::vec3 Camera::getCameraDirection(){
    return glm::normalize(m_Front);
}

void Camera::update(){
    
    m_Front.x = -sin(glm::radians(m_Yaw));
    m_Front.y = -sin(glm::radians(m_Pitch));
    m_Front.z = -cos(glm::radians(m_Yaw))*cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(m_Front);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    
}

Camera::~Camera(){

}