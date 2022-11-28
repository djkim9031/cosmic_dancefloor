#version 330

layout (location=0) in vec3 pos;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main(){
    TexCoords = pos;
    gl_Position = projection*view*vec4(pos, 1.f);
}