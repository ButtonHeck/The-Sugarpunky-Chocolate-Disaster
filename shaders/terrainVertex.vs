#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 instanceModel;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform bool instanceRendering;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out float PosHeight;

void main()
{
    mat4 Model;
    if (instanceRendering)
        Model = instanceModel;
    else
        Model = model;
    gl_Position = projection * view * Model * vec4(pos, 1.0);
    Normal = mat3(transpose(inverse(Model))) * normal;
    FragPos = vec3(Model * vec4(pos, 1.0));
    TexCoords = texCoords;
    PosHeight = pos.y;
}
