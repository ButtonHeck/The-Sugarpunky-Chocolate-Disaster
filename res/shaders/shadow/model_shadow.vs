#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 5) in mat4 i_model;

void main()
{
    gl_Position = i_model * i_pos;
}
