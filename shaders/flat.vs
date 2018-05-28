#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 instanceModel;

uniform mat4 projectionView;
uniform vec3 lightDirTo;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 LightDir;

void main()
{
    gl_Position = projectionView * instanceModel * vec4(pos, 1.0);
    FragPos = vec3(instanceModel * vec4(pos, 1.0));
    TexCoords = texCoords;
    LightDir = normalize(-lightDirTo);
}
