
# Cosmic Dancefloor #
Modern OpenGL with C++ for graphics rendering




https://user-images.githubusercontent.com/58758359/209041026-249877d9-e079-4dcf-a1d8-7253a0b30c0c.mp4




This is a personal project to utilize modern OpenGL for graphics rendering.
Vertex shaders, Geometry shader, Fragment shaders were used for this work

## Dependencies ##

GLEW 2.2.0 (For OpenGL 3.3): https://glew.sourceforge.net/ <br />
GLFW 3.3.8 (For GUI windows handling): https://github.com/glfw/glfw <br />
FreeType (For texts on screen): https://freetype.org/download.html <br />
ASSIMP 3.3.1 (For mesh model imports): https://github.com/assimp/assimp



GLM 0.9.9.8 and stb_image.h: These lightweight open source libraries are included in the current project, so you don't need to download/link them separately.


## Directory Structure ##
```
Root
|__ [assets]                <- Mesh model assets. .mtl, .obj, .stl
|__ [build]                 <- Project make files
|__ [fonts]                 <- Fonts for text displays on screen .ttf, .otf
|__ [includes]              <- Project hea````````````der files
|   |__ [glm]               <- Open source GLM library
|   |__ [lights]            <- For lighting related calculations
|   |__ [shadows]           <- For shadow related calculations
|   |__ [texts]             <- For text display on screen
|   |__ xxx.h               <- Regular other header files
|__ [shaders]               <- GLSL shaders
|__ [src]                   <- Project source files
|__ |__ [lights]            <- For lighting related calculations
|__ |__ [shadows]           <- For shadow related calculations
|__ |__ [texts]             <- For text display on screen
|__ |__ xxx.cpp             <- Regular other cpp files
|__ [textures]              <- Textures files to be linked to the mesh model assets. .jpg, .png
|   |__ [skybox]            <- Texture files to be linked to the skybox
|__ CMakeLists.txt          <- Project CMakeLists
|__ main.cpp                <- Main function
|__ readme.md               <- Project readme file        
```

## Implemented Functionalities ##

1. Custom mesh loading<br />
2. Directional lighting (based on Phong lighting - Ambient, Diffusional, Spectral)<br />
3. Point lighting<br />
4. Spot lighting + Flash light<br />
5. Directional shadow (shadowing cast by the directional lighting)<br />
6. Omnidirectional shadows (shadowing cast by point lights and spot lights)<br />
7. Skybox<br />

[TODO]<br />
1. Skeletal animation to improve realism in the scene<br />
2. Raytracing<br />
