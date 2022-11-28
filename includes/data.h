#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "shader.h"
#include "skybox.h"
//********************************************************************
//  This header file defines & loads all the data 
//  which is fed into the rendering pipeline
//********************************************************************

// Init data
const GLint WINDOW_WIDTH = 1280;
const GLint WINDOW_HEIGHT = 860;
const GLuint DIRECTIONAL_SHADOW_WIDTH = 4096;
const GLuint DIRECTIONAL_SHADOW_HEIGHT = 4096;
const GLuint OMNI_SHADOW_WIDTH = 2048;
const GLuint OMNI_SHADOW_HEIGHT = 2048;
const GLfloat OMNI_NEAR_PLANE = 0.01f;
const GLfloat OMNI_FAR_PLANE = 100.f;

// mathmatical values for animation
bool direction = true;
float currAngle = 0.f;

// shader file location
const char* vShader = "../shaders/shader.vert";
const char* fShader = "../shaders/shader.frag";

// shader uniform variables
GLuint uniformModel = 0;
GLuint uniformView = 0;
GLuint uniformProjection = 0;
GLuint uniformEyePosition = 0;
GLuint uniformSpecularIntensity = 0;
GLuint uniformShininess = 0;
GLuint uniformOmniLightPosition = 0;
GLuint uniformFarPlane = 0;

// shader list
std::vector<Shader*> shaders;

// FPS calculations
GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;

// lightings
unsigned int nPointLights = 0;
unsigned int nSpotLights = 0;

// just some random dance floor
unsigned int floorIndices[] = {
    0, 2, 1,
    1, 2, 3
};
GLfloat floorVertices[] = {
    -10.f, 0.f, -10.f,      0.f, 0.f,   0.f, -1.f, 0.f,
    10.f, 0.f, -10.f,       10.f,0.f,   0.f, -1.f, 0.f,
    -10.f, 0.f, 10.f,       0.f, 10.f,  0.f, -1.f, 0.f,
    10.f, 0.f, 10.f,        10.f, 10.f, 0.f, -1.f, 0.f
};

// skybox
Skybox skybox;

inline void CreateShader(){

    Shader* mainShader = new Shader();
    mainShader->create(vShader, fShader);
    shaders.push_back(mainShader);

    Shader* directionalShadowShader = new Shader();
    const char* vDirectionalShadowShader = "../shaders/directionalShadowMap.vert";
    const char* fDirectionalShadowShader = "../shaders/directionalShadowMap.frag";
    directionalShadowShader->create(vDirectionalShadowShader, fDirectionalShadowShader);
    shaders.push_back(directionalShadowShader);

    Shader* omniShadowShader = new Shader();
    const char* vOmniShadowShader = "../shaders/omniShadowMap.vert";
    const char* gOmniShadowShader  = "../shaders/omniShadowMap.geom";
    const char* fOmniShadowShader  = "../shaders/omniShadowMap.frag";
    omniShadowShader->create(vOmniShadowShader, gOmniShadowShader, fOmniShadowShader);
    shaders.push_back(omniShadowShader);

    Shader* textShader = new Shader();
    const char* vTextShader = "../shaders/texts.vert";
    const char* fTextShader = "../shaders/texts.frag";
    textShader->create(vTextShader, fTextShader);
    shaders.push_back(textShader);

}

inline void CreateSkybox(){

    std::vector<std::string> skyboxFaces;
    skyboxFaces.push_back("../textures/skybox/purplenebula_rt.tga");
    skyboxFaces.push_back("../textures/skybox/purplenebula_lf.tga");
    skyboxFaces.push_back("../textures/skybox/purplenebula_up.tga");
    skyboxFaces.push_back("../textures/skybox/purplenebula_dn.tga");
    skyboxFaces.push_back("../textures/skybox/purplenebula_bk.tga");
    skyboxFaces.push_back("../textures/skybox/purplenebula_ft.tga");

    skybox = Skybox(skyboxFaces);

}
