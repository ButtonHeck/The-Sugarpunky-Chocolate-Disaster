#version 450

layout (location = 0) in vec3 i_pos;

out vec3 v_FragPos;

void main()
{
    v_FragPos = i_pos;
}
