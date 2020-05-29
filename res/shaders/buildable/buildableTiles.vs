#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec4 i_translation;

uniform mat4 u_projectionView;

void main()
{
    gl_Position = u_projectionView * ( i_pos + i_translation );
}
