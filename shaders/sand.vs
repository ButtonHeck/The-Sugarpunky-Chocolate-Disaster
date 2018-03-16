#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out float PosHeight;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
    FragPos = vec3(model * vec4(pos, 1.0));
    PosHeight = pos.y;
}
