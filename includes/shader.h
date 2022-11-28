#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lights/directional_light.h"
#include "lights/point_light.h"
#include "lights/spot_light.h"

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

class Shader{

public:
    Shader();
    
    void create(const char* vShaderFile, const char* fShaderFile);
    void create(const char* vShaderFile, const char* gShaderFile, const char* fShaderFile);
    void use();
    void validate();
    void clear();

    //getters
    GLuint getModelLoc();
    GLuint getViewLoc();
    GLuint getProjectionLoc();
    GLuint getEyepositionLoc();

    GLuint getAmbientIntensityLoc();
    GLuint getAmbientColorLoc();
    GLuint getDiffuseIntensityLoc();

    GLuint getSpecularIntensityLoc();
    GLuint getShininessLoc();

    GLuint getOmniLightPositionLoc();
    GLuint getFarPlaneLoc();

    GLuint getTextsLoc();

    //setters
    void setTexture(GLuint textureUnit);

    void setDirectionalLight(DirectionalLight *dLight);
    void setPointLight(PointLight *pLight, unsigned int nPLights, unsigned int textureUnit, unsigned int offset);
    void setSpotLight(SpotLight *sLight, unsigned int nSLights, unsigned int textureUnit, unsigned int offset);

    void setDirectionalShadowMap(GLuint textureUnit);
    void setDirectionalLightSpaceTransform(glm::mat4* lTransform);

    void setLightMatrices(std::vector<glm::mat4> lightMatrices);
    
    ~Shader();

private:
    GLuint m_ShaderID;
    GLuint m_UniformModel, m_UniformView, m_UniformProjection;
    GLuint m_UniformEyePosition;
    GLuint m_UniformTexture;
    GLuint m_UniformSpecularIntensity, m_UniformShininess;
    GLuint m_UniformDirectionalShadowMap, m_UniformDirectionalLightSpaceTransform;
    GLuint m_UniformOmniLightPosition, m_UniformFarPlane;
    GLuint m_UniformLightMatrices[6];

    struct{
        GLuint s_UniformColor;
        GLuint s_UniformAmbientIntensity;
        GLuint s_UniformDiffuseIntensity;
        GLuint s_UniformDirection;
    } m_UniformDirectionalLight;

    int m_numPointLights, m_numSpotLights;
    GLuint m_UniformPointLightCount, m_UniformSpotLightCount;

    struct{
        GLuint s_UniformColor;
        GLuint s_UniformAmbientIntensity;
        GLuint s_UniformDiffuseIntensity;
        GLuint s_UniformPosition;

        GLuint s_UniformConstant;
        GLuint s_UniformLinear;
        GLuint s_UniformExponent;
    } m_UniformPointLight[MAX_POINT_LIGHTS];

    struct{
        GLuint s_UniformColor;
        GLuint s_UniformAmbientIntensity;
        GLuint s_UniformDiffuseIntensity;
        GLuint s_UniformPosition;

        GLuint s_UniformConstant;
        GLuint s_UniformLinear;
        GLuint s_UniformExponent;

        GLuint s_UniformDirection;
        GLuint s_UniformFOV;
    } m_UniformSpotLight[MAX_SPOT_LIGHTS];

    struct{
        GLuint s_UniformShadowMap;
        GLuint s_UniformFarPlane;
    } m_UniformOmniShadowMap[MAX_POINT_LIGHTS+MAX_SPOT_LIGHTS];

    //For texts
    GLuint m_UniformTexts;

    void add(GLuint program, const char* code, GLenum shaderType);
    void compile_shader(const char* vShader, const char* fShader);
    void compile_shader(const char* vShader, const char* gShader, const char* fShader);
    void compile();

    //helper function
    std::string readFile(const char* file);

};