#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window{

public:
    Window();
    Window(GLint winWidth, GLint winHeight);

    int init();

    GLfloat getBufferWidth() const { return (GLfloat) m_BufferWidth; }
    GLfloat getBufferHeight() const { return (GLfloat) m_BufferHeight; }
    bool shouldClose() const { return glfwWindowShouldClose(m_Window); }

    bool* getKeys() { return m_Keys; }
    GLfloat getXChange();
    GLfloat getYChange();

    void swapBuffers() { return glfwSwapBuffers(m_Window); }

    ~Window();

private:
    GLFWwindow* m_Window;
    GLint m_Width, m_Height;
    GLint m_BufferWidth, m_BufferHeight;

    bool m_Keys[1024];

    GLfloat m_LastX, m_LastY, m_xChange, m_yChange;
    bool m_MouseInitMotion;

    void CreateCallBack();
    static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void HandleMouse(GLFWwindow* window, double xPos, double yPos);

};