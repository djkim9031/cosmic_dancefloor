#include "mesh.h"

Mesh::Mesh(){

    m_VAO = 0;
    m_VBO = 0;
    m_IBO = 0;
    m_IndexCount = 0;

}

void Mesh::create(GLfloat* vertices, unsigned int* indices, 
                  unsigned int nVertices, unsigned int nIndices){

    m_IndexCount = nIndices;

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices*sizeof(indices[0]), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

    //Vertices data idx [0, 3) = x,y,z (position); idx [3, 5) = u, v (texture coord); [5, 8) = nx, ny, nz (normal vect)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Mesh::render(){

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Mesh::clear(){

    if(m_IBO!=0){
        glDeleteBuffers(1, &m_IBO);
        m_IBO = 0;
    }
    if(m_VBO!=0){
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if(m_VAO!=0){
        glDeleteBuffers(1, &m_VAO);
        m_VAO = 0;
    }

    m_IndexCount = 0;
}

Mesh::~Mesh(){
    clear();
}