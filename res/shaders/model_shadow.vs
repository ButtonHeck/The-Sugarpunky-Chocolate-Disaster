#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 5) in mat4 i_model;

uniform mat4 u_lightSpaceMatrix;

void main()
{
    gl_Position = u_lightSpaceMatrix * i_model * i_pos;
}
