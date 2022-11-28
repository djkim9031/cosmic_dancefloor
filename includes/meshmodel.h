#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include "mesh.h"
#include "texture.h"

#include <iostream>

class MeshModel{

public:
    MeshModel();

    //create mesh model from obj file
    void load(const std::string& modelFile, const char* defaultTex, TEXTURE_DATA_DEPTH_TYPE depthType);
    
    //create mesh model from a user defined GLfloat arrays
    void create_and_load(GLfloat* vertices, unsigned int* indices);

    void render();
    void render_custom(const unsigned int idx);
    void clear();

    ~MeshModel();

private:
    std::vector<Mesh*> m_Meshes, m_Meshes_Custom;
    std::vector<Texture*> m_Textures;
    std::vector<unsigned int> m_MeshToTex;

    void LoadNode(aiNode* node, const aiScene* scene);
    void LoadMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterials(const aiScene* scene, const char* defaultTex, TEXTURE_DATA_DEPTH_TYPE depthType);

    //helper function
    void calcAverageNormals(unsigned int* indices, unsigned int nIndexPoints, GLfloat* vertices, unsigned int nVertexPoints, unsigned int nOneVertexData, unsigned int offsetForNormals);

};