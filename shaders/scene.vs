#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in mat4 instanceModel;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform bool instanceRender;

out vec2 TexCoords;
out float PosHeight;

void main()
{
    if (instanceRender)
        gl_Position = projection * view * instanceModel * vec4(pos, 1.0);
    else
        gl_Position = projection * view * model * vec4(pos, 1.0);
    TexCoords = texCoords;
    PosHeight = pos.y;
}
