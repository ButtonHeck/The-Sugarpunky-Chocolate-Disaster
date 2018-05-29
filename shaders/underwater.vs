#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

uniform mat4 model;
uniform mat4 projectionView;
uniform vec3 lightDirTo;
uniform sampler2D underwater_normal;
uniform int tilesDimension;

out vec2 TexCoords;
out vec3 FragPos;
out float Diff;

void main()
{
    gl_Position = projectionView * model * vec4(pos, 1.0);
    FragPos = vec3(model * vec4(pos, 1.0));
    TexCoords = texCoords;
    vec3 LightDir = normalize(-lightDirTo);
    vec3 TexNormal = texture(underwater_normal, vec2(FragPos.x / (tilesDimension >> 2), FragPos.z / (tilesDimension >> 2))).rgb;
    vec3 Normal = normalize(TexNormal);
    Diff = max(dot(Normal, LightDir), 0.0);
}
