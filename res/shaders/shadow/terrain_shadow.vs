#version 450

layout (location = 0) in vec4 i_pos;

void main()
{
    vec4 pos = i_pos;
    pos.y -= 0.06;
    gl_Position = pos;
}
