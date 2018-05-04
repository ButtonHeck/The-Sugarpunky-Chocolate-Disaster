#version 450

layout (location = 0) in vec3 pos;

uniform mat4 projectionView;
uniform mat4 model;

void main()
{
    vec3 Pos = pos;
    Pos.y += 0.01;
    gl_Position = projectionView * model * vec4(Pos, 1.0);
}
