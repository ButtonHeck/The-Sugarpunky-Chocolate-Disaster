#version 450

layout (location = 0) in vec3 i_pos;

uniform mat4 u_projectionView;
uniform mat4 u_model;

void main()
{
    vec3 Pos = i_pos;
    Pos.y += 0.01;
    gl_Position = u_projectionView * u_model * vec4(Pos, 1.0);
}
