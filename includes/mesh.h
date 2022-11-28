#pragma once

#include <GL/glew.h>

class Mesh{

public:
    Mesh();

    void create(GLfloat* vertices, unsigned int* indices, unsigned int nVertices, unsigned int nIndices);
    void render();
    void clear();

    ~Mesh();

private:
    GLuint m_VAO, m_VBO, m_IBO;
    GLsizei m_IndexCount;

};