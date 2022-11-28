#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightMatrices[6];

out vec4 FragPos;

void main(){

    for(int face=0;face<6;++face){
        gl_Layer = face;
        for(int i=0;i<3;++i){
            FragPos = gl_in[i].gl_Position; //taking in a triangle with gl_in, and finding the gl_Position at that vertex
            gl_Position = lightMatrices[face]*FragPos; //converting the vertex point in the world coord to the light coord projected on one side of the cube
            EmitVertex(); //emitting vertex for the current triangle draw
        }
        EndPrimitive(); //given 3 emitted vertices, end the draw of the current triangle, and move to the next.
    }
}