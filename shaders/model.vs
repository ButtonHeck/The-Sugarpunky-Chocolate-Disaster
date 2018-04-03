#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in mat4 model;

uniform mat4 projectionView;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

void main()
{
    gl_Position = projectionView * model * vec4(pos, 1.0);
    TexCoords = texCoords;
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
}
