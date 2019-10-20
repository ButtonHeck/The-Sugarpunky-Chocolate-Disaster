#version 450

layout (location = 0) in vec4 i_pos;

uniform mat4 u_projectionView;
uniform vec4 u_translation;

void main()
{
    gl_Position = u_projectionView * (i_pos + u_translation);
}
