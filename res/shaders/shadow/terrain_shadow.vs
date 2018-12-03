#version 450

layout (location = 0) in vec4 i_pos;

void main()
{
    gl_Position = i_pos;
}
