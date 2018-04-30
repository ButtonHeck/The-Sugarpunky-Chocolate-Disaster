#version 450

layout (location = 0) in vec3 pos;

uniform mat4 projectionView;
uniform mat4 model;

void main()
{
    gl_Position = projectionView * model * vec4(pos, 1.0);
}
