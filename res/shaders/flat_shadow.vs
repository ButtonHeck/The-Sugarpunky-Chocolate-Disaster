#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 3) in mat4 i_instanceModel;

uniform mat4 u_lightSpaceMatrix;

void main()
{
    gl_Position = u_lightSpaceMatrix * i_instanceModel * vec4(i_pos, 1.0);
}
