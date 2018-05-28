#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 projectionView;
uniform vec3 lightDirTo;
uniform vec3 viewPosition;

out vec2 TexCoords;
out vec3 R;
out float Diff;
out float Spec;

void main()
{
    gl_Position = projectionView * model * vec4(pos, 1.0);
    vec3 Normal = normalize(mat3(transpose(inverse(model))) * normal);
    vec3 FragPos = vec3(model * vec4(pos, 1.0));
    TexCoords = texCoords;
    vec3 LightDir = normalize(-lightDirTo);
    vec3 I = normalize(FragPos - viewPosition);
    R = reflect(I, normalize(0.5 * Normal + vec3(0.0, 1.0, 0.0)));
    vec3 ViewDir = normalize(viewPosition - FragPos);
    Diff = max(dot(Normal, LightDir), 0.0);
    vec3 Reflect = reflect(-LightDir, Normal);
    Spec = pow(max(dot(Reflect, ViewDir), 0.0), 32.0);
}
