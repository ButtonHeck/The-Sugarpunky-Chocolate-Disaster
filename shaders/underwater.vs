#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoords;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    TexCoords = texCoords;
    FragPos = vec3(model * vec4(pos, 1.0));
}
