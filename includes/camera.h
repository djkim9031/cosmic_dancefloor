#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera{

public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 up, 
           GLfloat pitch, GLfloat yaw, GLfloat roll, 
           GLfloat translationalSpeed, GLfloat rotationalSpeed);
    
    void keyControl(bool *keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    glm::mat4 calculateViewMatrix();

    glm::vec3 getCameraPosition();
    glm::vec3 getCameraDirection();

    ~Camera();

private:
    glm::vec3 m_Position, m_Front, m_Up, m_Right, m_WorldUp;
    GLfloat m_Pitch, m_Yaw, m_Roll;
    GLfloat m_TranslationalSpeed, m_RotationalSpeed;

    void update();

};