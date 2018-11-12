#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;

out vec3 v_FragPos;
out vec3 v_normal;

void main()
{
    v_FragPos = i_pos;
    v_normal = i_normal;
}
