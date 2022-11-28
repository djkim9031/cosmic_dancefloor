#include "material.h"

Material::Material(){
    
    m_SpecularIntensity = 0.f;
    m_Shininess = 0.f;

}

Material::Material(GLfloat sIntensity, GLfloat sShininess){

    m_SpecularIntensity = sIntensity;
    m_Shininess = sShininess;

}

void Material::use(GLuint sIntensityLoc, GLuint sShininessLoc){

    glUniform1f(sIntensityLoc, m_SpecularIntensity);
    glUniform1f(sShininessLoc, m_Shininess);

}

Material::~Material(){
    
}