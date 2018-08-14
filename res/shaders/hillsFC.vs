#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;

out vec3 v_pos;
out vec2 v_texCoords;
out vec3 v_normal;

void main()
{
    v_pos = i_pos;
    v_texCoords = i_texCoords;
    v_normal = i_normal;
}
