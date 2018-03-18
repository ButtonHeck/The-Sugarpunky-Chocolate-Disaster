#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in mat4 instanceModel;

uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * instanceModel * vec4(pos, 1.0);
    TexCoords = texCoords;
}
