#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "shader.h"

#include "stb_init.h"

class Skybox{

public:
    Skybox();
    Skybox(std::vector<std::string> faceLocations);

    void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    ~Skybox();

private:
    Mesh* m_SkyMesh;
    Shader* m_SkyShader;

    GLuint m_TextureID;
    GLuint m_UniformProjection, m_UniformView;

};