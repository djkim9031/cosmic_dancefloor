#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "data.h"
#include "window.h"
#include "mesh.h"
#include "texture.h"
#include "meshmodel.h"
#include "camera.h"
#include "shader.h"
#include "material.h"
#include "lights/directional_light.h"
#include "lights/point_light.h"
#include "lights/spot_light.h"
#include "texts/text.h"

Camera camera;
MeshModel dancefloor, world, aya, cad;
Material shiny, dull;
Texture floorTex;

DirectionalLight mainLight;
PointLight pointLights[3];
SpotLight spotLights[3];


void RenderScene(){

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(0.f, -1.5f, 0.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shiny.use(uniformSpecularIntensity, uniformShininess);
    floorTex.use();
    dancefloor.render_custom(0);

    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(5.f, 0.f, -20.f));
    model = glm::scale(model, glm::vec3(2.f, 2.f, 2.f));
    model = glm::rotate(model, glm::radians(currAngle), glm::vec3(0.f, 1.f, 0.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shiny.use(uniformSpecularIntensity, uniformShininess);
    world.render();

    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    model = glm::rotate(model, glm::radians(3*currAngle), glm::vec3(-1.f, -1.f, -1.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shiny.use(uniformSpecularIntensity, uniformShininess);
    cad.render();

/*
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, -2.f, -1.f));
    model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shiny.use(uniformSpecularIntensity, uniformShininess);
    aya.render();
*/
 
}


void DirectionalShadowMapPass(DirectionalLight* dLight){

    shaders[1]->use();
    glViewport(0, 0, dLight->getShadowMap()->getShadowWidth(), dLight->getShadowMap()->getShadowHeight());
    dLight->getShadowMap()->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = shaders[1]->getModelLoc();
    glm::mat4 lightTransformMatrix = dLight->calculateLightTransform();
    shaders[1]->setDirectionalLightSpaceTransform(&lightTransformMatrix);

    shaders[1]->validate();
    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void OmniShadowMapPass(PointLight* pLight){

    shaders[2]->use();
    glViewport(0, 0, pLight->getShadowMap()->getShadowWidth(), pLight->getShadowMap()->getShadowHeight());
    pLight->getShadowMap()->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = shaders[2]->getModelLoc();
    uniformOmniLightPosition = shaders[2]->getOmniLightPositionLoc();
    uniformFarPlane = shaders[2]->getFarPlaneLoc();

    glUniform3f(uniformOmniLightPosition, pLight->getPosition().x, pLight->getPosition().y, pLight->getPosition().z);
    glUniform1f(uniformFarPlane, pLight->getFarPlane());
    shaders[2]->setLightMatrices(pLight->calculateLightTransform());

    shaders[2]->validate();
    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox.draw(viewMatrix, projectionMatrix);

    shaders[0]->use();
    uniformModel = shaders[0]->getModelLoc();
    uniformView = shaders[0]->getViewLoc();
    uniformProjection = shaders[0]->getProjectionLoc();
    uniformEyePosition = shaders[0]->getEyepositionLoc();
    uniformSpecularIntensity = shaders[0]->getSpecularIntensityLoc();
    uniformShininess = shaders[0]->getShininessLoc();

    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

    shaders[0]->setDirectionalLight(&mainLight);
    shaders[0]->setPointLight(pointLights, nPointLights, 3, 0);
    shaders[0]->setSpotLight(spotLights, nSpotLights, 3 + nPointLights, nPointLights);
    glm::mat4 lightTransformMatrix = mainLight.calculateLightTransform();
    shaders[0]->setDirectionalLightSpaceTransform(&lightTransformMatrix);

    mainLight.getShadowMap()->read(GL_TEXTURE2);
    shaders[0]->setTexture(1);
    shaders[0]->setDirectionalShadowMap(2);

    //setting a flash light
    glm::vec3 FlashPos = camera.getCameraPosition();
    FlashPos.y -= 1;
    spotLights[1].setFlash(FlashPos, camera.getCameraDirection());

    shaders[0]->validate();
    RenderScene();
}

int main(){

    Window win(WINDOW_WIDTH, WINDOW_HEIGHT);
    if(win.init()!=0){
        printf("[ERROR]\t\t Window setting went wrong.\n");
        return -1;
    }

    CreateShader();

    Text text("../fonts/ElsieSwashCaps-Black.otf", WINDOW_WIDTH, WINDOW_HEIGHT);
    text.init();

    camera = Camera(glm::vec3(0.f, 3.f, 10.f),glm::vec3(0.f, 1.f, 0.f), 10.f, 0.f, 0.f, 5.f, 0.1f);

    CreateSkybox();

    dancefloor = MeshModel();
    dancefloor.create_and_load(floorVertices, floorIndices);
    
    world = MeshModel();
    world.load("../assets/earth.obj", "../textures/earth.png", TEXTURE_DATA_DEPTH_TYPE::TEXTURE_DEPTH_RGBA);

    cad = MeshModel();
    cad.load("../assets/CAD_Model_Ver1.STL", "../textures/default.jpg", TEXTURE_DATA_DEPTH_TYPE::TEXTURE_DEPTH_RGB);

    //aya = MeshModel();
    //aya.load("../assets/aya.obj", "../textures/091_W_Aya_2K_01.jpg", TEXTURE_DATA_DEPTH_TYPE::TEXTURE_DEPTH_RGB);

    //Materials
    shiny = Material(1.f, 64.f);
    dull = Material(0.3f, 4.f);

    //Floor texture
    floorTex = Texture("../textures/default.jpg");
    floorTex.load_RGB();

    //Lightings
    mainLight = DirectionalLight(1.f, 1.f, 1.f, 0.2f, 0.5f, 10.5f, -20.f, -22.5f, DIRECTIONAL_SHADOW_WIDTH, DIRECTIONAL_SHADOW_HEIGHT);
    pointLights[0] = PointLight(0.f, 1.f, 0.f, 0.8f, 1.f, 0.f, 5.f, 0.f, 1.f, 0.01f, 0.1f, OMNI_SHADOW_WIDTH, OMNI_SHADOW_HEIGHT, OMNI_NEAR_PLANE, OMNI_FAR_PLANE);
    nPointLights++;
    spotLights[0] = SpotLight(0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 3.f, 3.f, 0.f, -1.f, -1.f, 1.f, 0.f, 0.0f, 20.f, OMNI_SHADOW_WIDTH, OMNI_SHADOW_HEIGHT, OMNI_NEAR_PLANE, OMNI_FAR_PLANE);
    nSpotLights++;
    spotLights[1] = SpotLight(1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 3.f, 3.f, 0.f, -1.f, -1.f, 1.f, 0.f, 0.0f, 20.f, OMNI_SHADOW_WIDTH, OMNI_SHADOW_HEIGHT, OMNI_NEAR_PLANE, OMNI_FAR_PLANE);
    nSpotLights++;

    glm::mat4 projection = glm::perspective(glm::radians(60.f), win.getBufferWidth()/win.getBufferHeight(), 0.1f, 100.f);


    while(!win.shouldClose()){

        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.keyControl(win.getKeys(), deltaTime);
        camera.mouseControl(win.getXChange(), win.getYChange());

        currAngle += 0.01f;
        if(currAngle>=360){
            currAngle = 0.f;
        }
        glDisable(GL_BLEND);
        
        DirectionalShadowMapPass(&mainLight);
        for(size_t i=0; i<nPointLights; ++i){
            OmniShadowMapPass(&pointLights[i]);
        }
        for(size_t i=0; i<nSpotLights; ++i){
            OmniShadowMapPass(&spotLights[i]);
        }
        RenderPass(projection, camera.calculateViewMatrix());
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    
        std::string fps = "FPS: " + std::to_string(1/deltaTime);
        text.render(shaders[3], fps, 30.f, WINDOW_HEIGHT - 50.f, 1.f, glm::vec3(3.f, 0.8f, 0.2f));
        
        glUseProgram(0);
        win.swapBuffers();

    }
 
    return 0;
}

