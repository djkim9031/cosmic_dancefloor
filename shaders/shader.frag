#version 330

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 DirectionalLightSpacePos;

struct Material{
    float specularIntensity;
    float shininess;
};

struct Light{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight{
    Light base;
    vec3 direction;
};

struct PointLight{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight{
    PointLight base;
    vec3 direction;
    float FOV;
};

struct OmniShadowMap{
    samplerCube shadowMap;
    float farPlane;
};

uniform sampler2D theTexture;
uniform vec3 eyePosition;
uniform Material material;
uniform DirectionalLight dLight;
uniform int numPointLights;
uniform int numSpotLights;
uniform PointLight pLights[MAX_POINT_LIGHTS];
uniform SpotLight sLights[MAX_SPOT_LIGHTS];
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS+MAX_SPOT_LIGHTS];

vec3 sampleOffsetDirections[20] = vec3[](
    vec3(1.f, 1.f, 1.f), vec3(1.f, -1.f, 1.f), vec3(-1.f, -1.f, 1.f), vec3(-1.f, 1.f, 1.f),
    vec3(1.f, 1.f, -1.f), vec3(1.f, -1.f, -1.f), vec3(-1.f, -1.f, -1.f), vec3(-1.f, 1.f, -1.f),
    vec3(1.f, 1.f, 0.f), vec3(1.f, -1.f, 0.f), vec3(-1.f, -1.f, 0.f), vec3(-1.f, 1.f, 0.f),
    vec3(1.f, 0.f, 1.f), vec3(-1.f, 0.f, 1.f), vec3(1.f, 0.f, -1.f), vec3(-1.f, 0.f, -1.f),
    vec3(0.f, 1.f, 1.f), vec3(0.f, -1.f, 1.f), vec3(0.f, -1.f, -1.f), vec3(0.f, 1.f, -1.f)
);

out vec4 color;

float CalcDirectionalShadowFactor(DirectionalLight dLight){

    //convert to normalized device coord [-1, +1]
    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
    //convert to the range [0, +1] to map to the depthMap range
    projCoords = (projCoords*0.5)+0.5;

    //Find the z vals closest to the directional light at each (x,y)
    //This point is lit by the light, and a value z that is bigger than this closest val must be shadow!
    //This works because currently this 3D map works in orthogonal projection.
    //If this was a perspective projection, comparing z values like this wouldn't work.
    //z range is [0, +1] = [closest, farthest]
    float closest = texture(directionalShadowMap, projCoords.xy).r;
    float current = projCoords.z;

    //Removing shadow acne
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(dLight.direction);
    float bias = max(0.001*(1-dot(normal, lightDir)), 0.0005);

    //Smoothing the pixellated edge with a Percentage Closer Filtering (PCF)
    float shadow = 0.f;
    vec2 texelSize = 1.f/textureSize(directionalShadowMap, 0); //finding one unit of texel size (u,v)
    for(int x=-2;x<=2;++x){
        for(int y=-2;y<=2;++y){
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y)*texelSize).r;
            shadow += current - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow = shadow/(25.f);

    //When the frag z values are farther away than the far plane of the ortho projection
    if(projCoords.z>1.0){
        shadow = 0.f;
    }

    return shadow;
}

float CalcOmniShadowFactor(PointLight pLight, int shadowIdx){

    vec3 lightToFrag = FragPos - pLight.position;
    float current = length(lightToFrag);

    float shadow = 0.f;
    float bias = 0.05;
    int samples = 20;
    float viewDistance = length(eyePosition - FragPos);
    float diskRadius = (1.0+(viewDistance/omniShadowMaps[shadowIdx].farPlane))/25.0;
    for(int i=0;i<samples;++i){
        float closest = texture(omniShadowMaps[shadowIdx].shadowMap, lightToFrag + sampleOffsetDirections[i]*diskRadius).r;
        closest *= omniShadowMaps[shadowIdx].farPlane; //from [0, +1] to actual dim (reverting what's done in omniShadowMap.frag)
        shadow += current - bias > closest ? 1.0 : 0.0;
    }
    shadow /= float(samples);

    return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor){

    //Ambient light 
    vec4 ambientColor = vec4(light.color, 1.f)*light.ambientIntensity;

    //Diffusional light
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.f);
    vec4 diffuseColor = vec4(light.color, 1.f)*light.diffuseIntensity*diffuseFactor;

    //Specular light
    vec4 specularColor = vec4(0.f, 0.f, 0.f, 0.f);
    if(diffuseFactor>0.f){
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedRay = normalize(reflect(direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedRay);
        if(specularFactor>0.f){
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color*material.specularIntensity*specularFactor, 1.f);
        }
    }

    return (ambientColor + (1.f - shadowFactor)*(diffuseColor + specularColor));
}

vec4 CalcPointLight(PointLight pLight, int shadowIdx){

    vec3 direction = FragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    float shadowFactor = CalcOmniShadowFactor(pLight, shadowIdx);

    vec4 color = CalcLightByDirection(pLight.base, direction, shadowFactor);
    float attenuation = pLight.exponent*distance*distance + pLight.linear*distance + pLight.constant;

    return (color/attenuation);
}

vec4 CalcSpotLight(SpotLight sLight, int shadowIdx){

    vec3 rayDirection = normalize(FragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);
    vec4 color = vec4(0.f, 0.f, 0.f, 0.f);
    if(slFactor > sLight.FOV){
        color = CalcPointLight(sLight.base, shadowIdx);
        color = color*(1.f - (1.f-slFactor)/(1.f-sLight.FOV));
    }

    return color;
}

vec4 CalcDirectionalLight(){
    float shadowFactor = CalcDirectionalShadowFactor(dLight);
    return CalcLightByDirection(dLight.base, dLight.direction, shadowFactor);
}

vec4 CalcPointLights(){

    vec4 total = vec4(0.f, 0.f, 0.f, 0.f);
    for(int i=0;i<numPointLights;++i){
        total+=CalcPointLight(pLights[i], i);
    }

    return total;
}

vec4 CalcSpotLights(){

    vec4 total = vec4(0.f, 0.f, 0.f, 0.f);
    for(int i=0;i<numSpotLights;++i){
        total+=CalcSpotLight(sLights[i], i+numPointLights);
    }

    return total;
}

void main(){

    vec4 lightAdjustment = CalcDirectionalLight();
    lightAdjustment += CalcPointLights();
    lightAdjustment += CalcSpotLights();

    color = texture(theTexture, TexCoord)*lightAdjustment;

}