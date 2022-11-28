#include "shader.h"

Shader::Shader(){

    m_ShaderID = 0;
    m_UniformModel = 0;
    m_UniformView = 0;
    m_UniformProjection = 0;
    m_UniformEyePosition = 0;
    m_UniformTexture = 0;
    m_UniformSpecularIntensity = 0;
    m_UniformShininess = 0;

    m_numPointLights = 0;
    m_numSpotLights = 0;
    m_UniformPointLightCount = 0;
    m_UniformSpotLightCount = 0;

    m_UniformDirectionalShadowMap = 0;
    m_UniformDirectionalLightSpaceTransform = 0;

    m_UniformOmniLightPosition = 0;
    m_UniformFarPlane = 0;

    m_UniformTexts = 0;

}

void Shader::create(const char* vShaderFile, const char* fShaderFile){

    std::string vShaderCode = readFile(vShaderFile);
    std::string fShaderCode = readFile(fShaderFile);
    compile_shader(vShaderCode.c_str(), fShaderCode.c_str());

}

void Shader::create(const char* vShaderFile, const char* gShaderFile, const char* fShaderFile){

    std::string vShaderCode = readFile(vShaderFile);
    std::string gShaderCode = readFile(gShaderFile);
    std::string fShaderCode = readFile(fShaderFile);
    compile_shader(vShaderCode.c_str(), gShaderCode.c_str(), fShaderCode.c_str());

}

void Shader::use(){
    glUseProgram(m_ShaderID);
}

void Shader::validate(){

    GLint result = 0;
    GLchar eLog[1024] = {0};
    glValidateProgram(m_ShaderID);
    glGetProgramiv(m_ShaderID, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(m_ShaderID, sizeof(eLog), NULL, eLog);
        printf("[ERROR]\t\t Validating shader program: '%s'\n", eLog);
        return;
    }

}

void Shader::add(GLuint program, const char* code, GLenum shaderType){

    GLuint currShader = glCreateShader(shaderType);

    const GLchar* shaderCode = code;
    GLint codeLength = strlen(code);

    glShaderSource(currShader, 1, &shaderCode, &codeLength);
    glCompileShader(currShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(currShader, GL_COMPILE_STATUS, &result);
    if(!result){
        glGetShaderInfoLog(currShader, sizeof(eLog), NULL, eLog);
        printf("[ERROR]\t\t %d shader compilation: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(program, currShader);
    return;
}

void Shader::compile_shader(const char* vShader, const char* fShader){

    m_ShaderID = glCreateProgram();
    if(!m_ShaderID){
        printf("[ERROR]\t\t Shader program creation\n");
        return;
    }

    add(m_ShaderID, vShader, GL_VERTEX_SHADER);
    add(m_ShaderID, fShader, GL_FRAGMENT_SHADER);

    compile();

    return;
}

void Shader::compile_shader(const char* vShader, const char* gShader, const char* fShader){

    m_ShaderID = glCreateProgram();
    if(!m_ShaderID){
        printf("[ERROR]\t\t Shader program creation\n");
        return;
    }

    add(m_ShaderID, vShader, GL_VERTEX_SHADER);
    add(m_ShaderID, gShader, GL_GEOMETRY_SHADER);
    add(m_ShaderID, fShader, GL_FRAGMENT_SHADER);

    compile();
    
    return;
}

void Shader::compile(){

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(m_ShaderID);
    glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(m_ShaderID, sizeof(eLog), NULL, eLog);
        printf("[ERROR]\t\t Linking shader program: '%s'\n", eLog);
        return;
    }

    m_UniformModel = glGetUniformLocation(m_ShaderID, "model");
    m_UniformView = glGetUniformLocation(m_ShaderID, "view");
    m_UniformProjection = glGetUniformLocation(m_ShaderID, "projection");
    m_UniformEyePosition = glGetUniformLocation(m_ShaderID, "eyePosition");
    m_UniformTexture = glGetUniformLocation(m_ShaderID, "theTexture");
    m_UniformSpecularIntensity = glGetUniformLocation(m_ShaderID, "material.specularIntensity");
    m_UniformShininess = glGetUniformLocation(m_ShaderID, "material.shininess");

    m_UniformDirectionalLight.s_UniformAmbientIntensity = glGetUniformLocation(m_ShaderID, "dLight.base.ambientIntensity");
    m_UniformDirectionalLight.s_UniformColor = glGetUniformLocation(m_ShaderID, "dLight.base.color");
    m_UniformDirectionalLight.s_UniformDiffuseIntensity = glGetUniformLocation(m_ShaderID, "dLight.base.diffuseIntensity");
    m_UniformDirectionalLight.s_UniformDirection = glGetUniformLocation(m_ShaderID, "dLight.direction");

    m_UniformPointLightCount = glGetUniformLocation(m_ShaderID, "numPointLights");
    for(unsigned int i=0;i<MAX_POINT_LIGHTS;++i){
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "pLights[%d].base.ambientIntensity", i);
        m_UniformPointLight[i].s_UniformAmbientIntensity = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pLights[%d].base.color", i);
        m_UniformPointLight[i].s_UniformColor = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pLights[%d].base.diffuseIntensity", i);
        m_UniformPointLight[i].s_UniformDiffuseIntensity = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pLights[%d].position", i);
        m_UniformPointLight[i].s_UniformPosition = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pLights[%d].constant", i);
        m_UniformPointLight[i].s_UniformConstant = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pLights[%d].linear", i);
        m_UniformPointLight[i].s_UniformLinear = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pLights[%d].exponent", i);
        m_UniformPointLight[i].s_UniformExponent = glGetUniformLocation(m_ShaderID, locBuff);
        
    }

    m_UniformSpotLightCount = glGetUniformLocation(m_ShaderID, "numSpotLights");
    for(unsigned int i=0;i<MAX_SPOT_LIGHTS;++i){
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.base.ambientIntensity", i);
        m_UniformSpotLight[i].s_UniformAmbientIntensity = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.base.color", i);
        m_UniformSpotLight[i].s_UniformColor = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.base.diffuseIntensity", i);
        m_UniformSpotLight[i].s_UniformDiffuseIntensity = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.position", i);
        m_UniformSpotLight[i].s_UniformPosition = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.constant", i);
        m_UniformSpotLight[i].s_UniformConstant = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.linear", i);
        m_UniformSpotLight[i].s_UniformLinear = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.exponent", i);
        m_UniformSpotLight[i].s_UniformExponent = glGetUniformLocation(m_ShaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "sLights[%d].direction", i);
        m_UniformSpotLight[i].s_UniformDirection = glGetUniformLocation(m_ShaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "sLights[%d].FOV", i);
        m_UniformSpotLight[i].s_UniformFOV = glGetUniformLocation(m_ShaderID, locBuff);
        
    }

    m_UniformDirectionalShadowMap = glGetUniformLocation(m_ShaderID, "directionalShadowMap");
    m_UniformDirectionalLightSpaceTransform = glGetUniformLocation(m_ShaderID, "directionalLightSpaceTransform");

    m_UniformOmniLightPosition = glGetUniformLocation(m_ShaderID, "lightPos");
    m_UniformFarPlane = glGetUniformLocation(m_ShaderID, "farPlane");

    for(unsigned int  i=0; i<6; ++i){
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
        m_UniformLightMatrices[i] = glGetUniformLocation(m_ShaderID, locBuff);
    }

    for(unsigned int  i=0; i<MAX_POINT_LIGHTS+MAX_SPOT_LIGHTS; ++i){
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
        m_UniformOmniShadowMap[i].s_UniformShadowMap = glGetUniformLocation(m_ShaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
        m_UniformOmniShadowMap[i].s_UniformFarPlane = glGetUniformLocation(m_ShaderID, locBuff);
    }

    //Texts
    m_UniformTexts = glGetUniformLocation(m_ShaderID, "textColor");

    return;
}

void Shader::clear(){

    if(m_ShaderID!=0){
        glDeleteProgram(m_ShaderID);
        m_ShaderID = 0;
    }

    m_ShaderID = 0;
    m_UniformModel = 0;
    m_UniformView = 0;
    m_UniformProjection = 0;
    m_UniformEyePosition = 0;
    m_UniformTexture = 0;
    m_UniformSpecularIntensity = 0;
    m_UniformShininess = 0;

    m_numPointLights = 0;
    m_numSpotLights = 0;
    m_UniformPointLightCount = 0;
    m_UniformSpotLightCount = 0;

    m_UniformDirectionalShadowMap = 0;
    m_UniformDirectionalLightSpaceTransform = 0;

    m_UniformOmniLightPosition = 0;
    m_UniformFarPlane = 0;

    m_UniformTexts = 0;

}

GLuint Shader::getModelLoc(){
    return m_UniformModel;
}

GLuint Shader::getViewLoc(){
    return m_UniformView;
}

GLuint Shader::getProjectionLoc(){
    return m_UniformProjection;
}

GLuint Shader::getEyepositionLoc(){
    return m_UniformEyePosition;
}

GLuint Shader::getAmbientIntensityLoc(){
    return m_UniformDirectionalLight.s_UniformAmbientIntensity;
}

GLuint Shader::getAmbientColorLoc(){
    return m_UniformDirectionalLight.s_UniformColor;
}

GLuint Shader::getDiffuseIntensityLoc(){
    return m_UniformDirectionalLight.s_UniformDiffuseIntensity;
}

GLuint Shader::getSpecularIntensityLoc(){
    return m_UniformSpecularIntensity;
}

GLuint Shader::getShininessLoc(){
    return m_UniformShininess;
}

GLuint Shader::getOmniLightPositionLoc(){
    return m_UniformOmniLightPosition;
}

GLuint Shader::getFarPlaneLoc(){
    return m_UniformFarPlane;
}

GLuint Shader::getTextsLoc(){
    return m_UniformTexts;
}

void Shader::setTexture(GLuint textureUnit){
    glUniform1i(m_UniformTexture, textureUnit);
}

void Shader::setDirectionalLight(DirectionalLight *dLight){

    dLight->use(m_UniformDirectionalLight.s_UniformAmbientIntensity, 
                m_UniformDirectionalLight.s_UniformColor,
                m_UniformDirectionalLight.s_UniformDiffuseIntensity,
                m_UniformDirectionalLight.s_UniformDirection);

}

void Shader::setPointLight(PointLight *pLight, unsigned int nPLights, unsigned int textureUnit, unsigned int offset){

    if(nPLights > MAX_POINT_LIGHTS){
        m_numPointLights = MAX_POINT_LIGHTS;
    } else{
        m_numPointLights = nPLights;
    }

    glUniform1i(m_UniformPointLightCount, m_numPointLights);
    for(size_t i=0;i<m_numPointLights;++i){
        pLight[i].use(m_UniformPointLight[i].s_UniformAmbientIntensity, m_UniformPointLight[i].s_UniformColor,
                      m_UniformPointLight[i].s_UniformDiffuseIntensity, m_UniformPointLight[i].s_UniformPosition,
                      m_UniformPointLight[i].s_UniformConstant, m_UniformPointLight[i].s_UniformLinear, m_UniformPointLight[i].s_UniformExponent);
    
        pLight[i].getShadowMap()->read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(m_UniformOmniShadowMap[i+offset].s_UniformShadowMap, textureUnit+i);
        glUniform1f(m_UniformOmniShadowMap[i+offset].s_UniformFarPlane, pLight[i].getFarPlane());
    }

}

void Shader::setSpotLight(SpotLight *sLight, unsigned int nSLights, unsigned int textureUnit, unsigned int offset){

    if(nSLights > MAX_SPOT_LIGHTS){
        m_numSpotLights = MAX_SPOT_LIGHTS;
    } else{
        m_numSpotLights = nSLights;
    }

    glUniform1i(m_UniformSpotLightCount, m_numSpotLights);
    for(size_t i=0;i<m_numSpotLights;++i){
        sLight[i].use(m_UniformSpotLight[i].s_UniformAmbientIntensity, m_UniformSpotLight[i].s_UniformColor,
                      m_UniformSpotLight[i].s_UniformDiffuseIntensity, m_UniformSpotLight[i].s_UniformPosition,
                      m_UniformSpotLight[i].s_UniformDirection, 
                      m_UniformSpotLight[i].s_UniformConstant, m_UniformSpotLight[i].s_UniformLinear, m_UniformSpotLight[i].s_UniformExponent,
                      m_UniformSpotLight[i].s_UniformFOV);

        sLight[i].getShadowMap()->read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(m_UniformOmniShadowMap[i+offset].s_UniformShadowMap, textureUnit+i);
        glUniform1f(m_UniformOmniShadowMap[i+offset].s_UniformFarPlane, sLight[i].getFarPlane());
    }
}

void Shader::setDirectionalShadowMap(GLuint textureUnit){
    glUniform1i(m_UniformDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightSpaceTransform(glm::mat4* lTransform){
    glUniformMatrix4fv(m_UniformDirectionalLightSpaceTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::setLightMatrices(std::vector<glm::mat4> lightMatrices){
    for(size_t i=0; i<6; ++i){
        glUniformMatrix4fv(m_UniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
    }
}
    
std::string Shader::readFile(const char* file){

    std::string content;
    std::ifstream filestream(file, std::ios::in);

    if(!filestream.is_open()){
        printf("[ERROR]\t\t %s cannot be loaded.\n", file);
        return "";
    }

    std::string line = "";
    while(!filestream.eof()){
        std::getline(filestream, line);
        content.append(line + "\n");
    }

    filestream.close();
    
    return content;
}

Shader::~Shader(){
    clear();
}