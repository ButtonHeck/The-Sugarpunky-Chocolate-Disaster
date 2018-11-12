#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec3 i_normal;

uniform mat4 u_projectionView;

out vec3 v_Normal;

void main()
{
    gl_Position = u_projectionView * i_pos;
    v_Normal = i_normal * 0.2;
}
