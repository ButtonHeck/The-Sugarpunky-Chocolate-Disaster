/*
this shader is used only for visual debug purposes
and is to be cut out from the project in release time
*/

#version 450

layout (location = 0) in vec3 i_pos;

uniform mat4 u_projectionView;

void main()
{
    gl_Position = u_projectionView * vec4(i_pos, 1.0);
}
