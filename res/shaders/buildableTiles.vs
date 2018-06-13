#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 3) in mat4 i_model;

uniform mat4 u_projectionView;

void main()
{
    gl_Position = u_projectionView * i_model * vec4(i_pos, 1.0);
}
