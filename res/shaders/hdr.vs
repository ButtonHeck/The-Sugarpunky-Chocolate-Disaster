#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = i_texCoords;
    gl_Position = vec4(i_pos, 1.0);
}
