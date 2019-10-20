#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 0, component = 3) in float i_animationOffset;

out vec4 v_FragPos;

void main()
{
    v_FragPos = vec4(i_pos, i_animationOffset);
}
