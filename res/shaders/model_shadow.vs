#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 5) in mat4 i_model;

uniform mat4 u_lightSpaceMatrix;

const float SCALE_FACTOR = 1.04;

void main()
{
    mat4 scaleMatrix = mat4(1.0) * SCALE_FACTOR;
    scaleMatrix[3][3] = 1.0;
    mat4 scaledModel = i_model * scaleMatrix;
    gl_Position = u_lightSpaceMatrix * scaledModel * i_pos;
}
