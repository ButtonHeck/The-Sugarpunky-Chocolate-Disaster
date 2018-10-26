#version 450

layout (location = 0) in vec4 i_pos;

uniform mat4 u_model;
uniform mat4 u_projectionView;

void main()
{
    gl_Position = (u_projectionView * u_model * i_pos).xyww;
}
