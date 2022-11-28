#version 330

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 directionalLightSpaceTransform;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec4 DirectionalLightSpacePos;

void main(){

    gl_Position = projection*view*model*vec4(pos, 1.f);
    DirectionalLightSpacePos = directionalLightSpaceTransform*model*vec4(pos, 1.f);
    TexCoord = tex;
    FragPos = (model*vec4(pos, 1.f)).xyz;

    //To preserve the normal vector's orientation even when scaling happens,
    Normal = mat3(transpose(inverse(model)))*norm;

}