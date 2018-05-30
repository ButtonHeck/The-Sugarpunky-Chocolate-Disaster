#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_direction;
layout (location = 2) in vec3 i_color;

out vec3 v_gColor;
out vec3 v_gDirection;

uniform mat4  u_view;
uniform float u_aspectRatio;

void main()
{
    gl_Position = vec4(i_pos, 1.0);
    v_gColor = i_color;
    v_gDirection = mat3(u_view) * i_direction;
    v_gDirection.x /= u_aspectRatio;
}
