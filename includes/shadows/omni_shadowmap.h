#pragma once
#include "shadowmap.h"

class OmniShadowMap : public ShadowMap{

public:

    OmniShadowMap();
    bool init(GLuint width, GLuint height);
    void write();
    void read(GLenum textureUnit);

    ~OmniShadowMap();

private:

};