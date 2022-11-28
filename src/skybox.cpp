#include "skybox.h"

Skybox::Skybox(){

}

Skybox::Skybox(std::vector<std::string> faceLocations){

    //Shader setup
    m_SkyShader = new Shader();
    m_SkyShader->create("../shaders/skybox.vert", "../shaders/skybox.frag");
    m_UniformProjection = m_SkyShader->getProjectionLoc();
    m_UniformView = m_SkyShader->getViewLoc();

    //Texture setup
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    int width, height, bitDepth;
    for(size_t i=0;i<6;++i){
        unsigned char* texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
        if(!texData){
            printf("[ERROR-Skybox]\t Texture file at %s cannot be loaded.\n", faceLocations[i].c_str());
            return;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        stbi_image_free(texData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Mesh setup
    unsigned int skyboxIndices[] = {
        //front
        0, 1, 2,
        2, 1, 3,
        //right
        2, 3, 5,
        5, 3, 7,
        //back
        5, 7, 4,
        4, 7, 6,
        //left
        4, 6, 0,
        0, 6, 1,
        //top
        4, 0, 5,
        5, 0, 2,
        //bottom
        1, 6, 3,
        6, 3, 7
    };

    float skyboxVertices[] = {
        -1.f, 1.f, -1.f,    0.f, 0.f,   0.f, 0.f, 0.f,
        -1.f, -1.f, -1.f,   0.f, 0.f,   0.f, 0.f, 0.f,
        1.f, 1.f, -1.f,     0.f, 0.f,   0.f, 0.f, 0.f,
        1.f, -1.f, -1.f,    0.f, 0.f,   0.f, 0.f, 0.f,

        -1.f, 1.f, 1.f,     0.f, 0.f,   0.f, 0.f, 0.f,
        1.f, 1.f, 1.f,      0.f, 0.f,   0.f, 0.f, 0.f,
        -1.f, -1.f, 1.f,    0.f, 0.f,   0.f, 0.f, 0.f,
        1.f, -1.f, 1.f,     0.f, 0.f,   0.f, 0.f, 0.f
    };

    m_SkyMesh = new Mesh();
    m_SkyMesh->create(skyboxVertices, skyboxIndices, 64, 36);

}

void Skybox::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix){

    //Removing the 4th column to disregard the translational info
    //Only rotational/scaling info are needed for skybox with respect to the camera motion
    viewMatrix = glm::mat4(glm::mat3(viewMatrix));

    glDepthMask(GL_FALSE);

    m_SkyShader->use();
    glUniformMatrix4fv(m_UniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(m_UniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    m_SkyShader->validate();
    m_SkyMesh->render();

    glDepthMask(GL_TRUE);

}

Skybox::~Skybox(){

}