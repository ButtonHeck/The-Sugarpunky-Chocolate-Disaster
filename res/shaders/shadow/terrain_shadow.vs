#version 450

layout (location = 0) in vec4 i_pos;

void main()
{
    vec4 pos = i_pos;
    //slightly change y coord for smoother shadows at surface edges
    pos.y -= 0.06;
    gl_Position = pos;
}
