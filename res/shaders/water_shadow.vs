#version 450

layout (location = 0) in vec3 i_pos;

uniform mat4 u_lightSpaceMatrix;

void main()
{
    vec4 position = u_lightSpaceMatrix * vec4(i_pos, 1.0);
    position.z = 1.0;
    gl_Position = position;
}
