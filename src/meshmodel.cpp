#include "meshmodel.h"

MeshModel::MeshModel(){

}

void MeshModel::load(const std::string& modelFile, const char* defaultTex, TEXTURE_DATA_DEPTH_TYPE depthType){

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if(!scene){
        printf("[ERROR]\t\t Model file at %s cannot be loaded: %s\n", modelFile.c_str(), importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene);
    LoadMaterials(scene, defaultTex, depthType);

}

void calcAverageNormals(unsigned int* indices, unsigned int nIndexPoints, GLfloat* vertices, unsigned int nVertexPoints, unsigned int nOneVertexData, unsigned int offsetForNormals){

    for(size_t i=0;i<nIndexPoints;i+=3){
        unsigned int in0 = indices[i]*nOneVertexData;
        unsigned int in1 = indices[i+1]*nOneVertexData;
        unsigned int in2 = indices[i+2]*nOneVertexData;

        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1+1] - vertices[in0+1], vertices[in1+2] - vertices[in0+2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2+1] - vertices[in0+1], vertices[in2+2] - vertices[in0+2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        in0 += offsetForNormals; in1 += offsetForNormals; in2 += offsetForNormals;
        vertices[in0] += normal.x; vertices[in0+1] += normal.y; vertices[in0+2] += normal.z;
        vertices[in1] += normal.x; vertices[in1+1] += normal.y; vertices[in1+2] += normal.z;
        vertices[in2] += normal.x; vertices[in2+1] += normal.y; vertices[in2+2] += normal.z;
    }

    for(size_t i=0;i<nVertexPoints/nOneVertexData; ++i){
        unsigned int nOffset = i*nOneVertexData + offsetForNormals;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset+1], vertices[nOffset+2]);
        vec = glm::normalize(vec);

        vertices[nOffset] = vec.x; vertices[nOffset+1] = vec.y; vertices[nOffset+2] = vec.z;
    }
}


void MeshModel::create_and_load(GLfloat* vertices, unsigned int* indices){

    //calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* meshObject = new Mesh();
    meshObject->create(vertices, indices, 32, 6);
    m_Meshes_Custom.push_back(meshObject);

}

void MeshModel::LoadNode(aiNode* node, const aiScene* scene){

    for(size_t i=0;i<node->mNumMeshes;++i){
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for(size_t i=0;i<node->mNumChildren;++i){
        LoadNode(node->mChildren[i], scene);
    }

}

void MeshModel::LoadMesh(aiMesh* mesh, const aiScene* scene){

    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    //As defined in mesh.cpp, Vertices data idx [0, 3) = x,y,z (position); idx [3, 5) = u, v (texture coord); [5, 8) = nx, ny, nz (normal vect)
    for(size_t i=0;i<mesh->mNumVertices;++i){
        vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
        if(mesh->mTextureCoords[0]){
            //if the obj file has a texture coord data
            vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
        } else{
            //if the obj file doesn't have a texture coord data
            vertices.insert(vertices.end(), {0.f, 0.f});
        }
        //Use the inverted normal vectors so lighting calculation is intuitive
        glm::vec3 nVec(-mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z);
        nVec = glm::normalize(nVec);
        vertices.insert(vertices.end(), {nVec.x, nVec.y, nVec.z});
    }

    //corresponding indices for the vertices above
    for(size_t i=0;i<mesh->mNumFaces;++i){
        aiFace face = mesh->mFaces[i];
        for(size_t j=0;j<face.mNumIndices;++j){
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* meshData = new Mesh();
    meshData->create(&vertices[0], &indices[0], vertices.size(), indices.size());
    m_Meshes.push_back(meshData);
    m_MeshToTex.push_back(mesh->mMaterialIndex);

}

void MeshModel::LoadMaterials(const aiScene* scene, const char* defaultTex, TEXTURE_DATA_DEPTH_TYPE depthType){

    m_Textures.resize(scene->mNumMaterials);

    for(size_t i=0;i<scene->mNumMaterials;++i){
        aiMaterial* material = scene->mMaterials[i];
        m_Textures[i] = nullptr;

        if(material->GetTextureCount(aiTextureType_DIFFUSE)){
            aiString path;
            if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path)==AI_SUCCESS){
                int idx = std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(idx+1);
                std::string texPath = std::string("textures/") + filename;

                m_Textures[i] = new Texture(texPath.c_str());

                if(!m_Textures[i]->load_RGB()){
                    printf("[ERROR]\t\t Texture file at %s cannot be loaded\n", texPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = nullptr;
                }
            }
        }

        if(!m_Textures[i]){
            //use the default texture if the texture doesn't exist
            m_Textures[i] = new Texture(defaultTex);
            if(depthType==TEXTURE_DATA_DEPTH_TYPE::TEXTURE_DEPTH_RGB){
                m_Textures[i]->load_RGB();
            }else{
                m_Textures[i]->load_RGBA();
            }
        }
    }

}

void MeshModel::render(){

    for(size_t i=0;i<m_Meshes.size();++i){
        unsigned int materialIndex = m_MeshToTex[i];
        if(materialIndex < m_Textures.size() && m_Textures[materialIndex]){
            m_Textures[materialIndex]->use();
        }

        m_Meshes[i]->render();
    }

}

void MeshModel::render_custom(const unsigned int idx){

    if(idx>=m_Meshes_Custom.size()){
        return;                    
    }

    m_Meshes_Custom[idx]->render();

}

void MeshModel::clear(){

    for(size_t i=0;i<m_Meshes.size();++i){
        if(m_Meshes[i]){
            delete m_Meshes[i];
            m_Meshes[i] = nullptr;
        }
    }

    for(size_t i=0;i<m_Meshes_Custom.size();++i){
        if(m_Meshes_Custom[i]){
            delete m_Meshes_Custom[i];
            m_Meshes_Custom[i] = nullptr;
        }
    }

    for(size_t i=0;i<m_Textures.size();++i){
        if(m_Textures[i]){
            delete m_Textures[i];
            m_Textures[i] = nullptr;
        }
    }

    m_Meshes.clear();
    m_Meshes_Custom.clear();
    m_Textures.clear();
    m_MeshToTex.clear();

}

MeshModel::~MeshModel(){
    clear();
}

