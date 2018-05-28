#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in mat4 model;

uniform mat4 projectionView;
uniform vec3 viewPosition;
uniform vec3 lightDirTo;

out vec2 TexCoords;
out vec3 Normal;
out float Diff;
out float Spec;

void main()
{
    gl_Position = projectionView * model * vec4(pos, 1.0);
    TexCoords = texCoords;
    vec3 FragPos = vec3(model * vec4(pos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model))) * normal);
    vec3 ViewDir = normalize(viewPosition - FragPos);
    vec3 LightDir = normalize(-lightDirTo);
    Diff = max(dot(Normal, LightDir), 0.0);
    vec3 Reflect = reflect(-LightDir, Normal);
    Spec = pow(max(dot(Reflect, ViewDir), 0.0), 8.0) * 0.33;
}
