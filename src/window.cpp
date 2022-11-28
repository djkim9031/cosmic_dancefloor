#include "window.h"

Window::Window(){

    m_Width = 800;
    m_Height = 600;
    for(size_t i=0;i<1024;++i){
        m_Keys[i] = false;
    }
    m_MouseInitMotion = true;
    m_xChange = 0.f;
    m_yChange = 0.f;

}

Window::Window(GLint winWidth, GLint winHeight){

    m_Width = winWidth;
    m_Height = winHeight;
    for(size_t i=0;i<1024;++i){
        m_Keys[i] = false;
    }
    m_MouseInitMotion = true;
    m_xChange = 0.f;
    m_yChange = 0.f;

}

int Window::init(){

    if(!glfwInit()){
        printf("[ERROR]\t\t GLFW initialization\n");
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    m_Window = glfwCreateWindow(m_Width, m_Height, "Cosmic Dancefloor", NULL, NULL);
    if(!m_Window){
        printf("[ERROR]\t\t Window creation\n");
        glfwTerminate();
        return -1;
    }

    glfwGetFramebufferSize(m_Window, &m_BufferWidth, &m_BufferHeight);
    glfwMakeContextCurrent(m_Window);
    CreateCallBack();

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        printf("[ERROR]\t\t GLEW initialization\n");
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, m_BufferWidth, m_BufferHeight);
    glfwSetWindowUserPointer(m_Window, this);

    return 0;
}



void Window::CreateCallBack(){

    glfwSetKeyCallback(m_Window, HandleKeys);
    glfwSetCursorPosCallback(m_Window, HandleMouse);

}

GLfloat Window::getXChange(){

    GLfloat change = m_xChange;
    m_xChange = 0.f;
    return change;
}

GLfloat Window::getYChange(){

    GLfloat change = m_yChange;
    m_yChange = 0.f;
    return change;
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode){
    
    Window* currWin = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if( (key==GLFW_KEY_ESCAPE || key==GLFW_KEY_Q) && action==GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }

    if(key>=0 && key<1024){
        if(action == GLFW_PRESS){
            currWin->m_Keys[key] = true;
        } else if(action == GLFW_RELEASE){
            currWin->m_Keys[key] = false;
        }
    }
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos){

    Window* currWin = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(currWin->m_MouseInitMotion){
        currWin->m_LastX = xPos;
        currWin->m_LastY = yPos;
        currWin->m_MouseInitMotion = false;
    }

    currWin->m_xChange = xPos - currWin->m_LastX;
    currWin->m_yChange = currWin->m_LastY - yPos;
    currWin->m_LastX = xPos;
    currWin->m_LastY = yPos;

}

Window::~Window(){
    
    glfwDestroyWindow(m_Window);
    glfwTerminate();

}