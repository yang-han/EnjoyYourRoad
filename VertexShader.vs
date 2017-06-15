#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace; //aPos
layout(location = 1) in vec2 vertexUV; //aTexCoords
layout(location = 2) in vec3 vertexNormal_modelspace; //aNormal

//// Output data ; will be interpolated for each fragment.
//out vec2 UV; //vs_out.TexCoords
//out vec3 Position_worldspace;//vs_out.FragPos
//out vec3 Normal_cameraspace; //vs_out.Normal
//out vec3 EyeDirection_cameraspace; //
//out vec3 LightDirection_cameraspace;
//out vec4 ShadowCoord;
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

// Values that stay constant for the whole mesh.
uniform mat4 P; //
uniform mat4 V; //view
uniform mat4 M; //model
//uniform vec3 LightPosition_worldspace;//lightSpaceMatrix
uniform mat4 lightSpaceMatrix;

void main(){
    
    vs_out.FragPos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
    vs_out.Normal = transpose(inverse(mat3(M))) * vertexNormal_modelspace;
    vs_out.TexCoords = vertexUV;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0);
    
}

