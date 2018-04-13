#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 direction;
layout (location = 2) in vec3 color;

out vec3 gColor;
out vec3 gDirection;

uniform mat4 view;
uniform float aspectRatio;

void main()
{
    gl_Position = vec4(pos, 1.0);
    gColor = color;
    gDirection = mat3(view) * direction;
    gDirection.x /= aspectRatio;
}
