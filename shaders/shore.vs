#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 projectionView;
uniform vec3 lightDirTo;
uniform sampler2D base_normal;
uniform int tilesDimension;

out vec2 TexCoords;
out vec3 FragPos;
out float PosHeight;
out float TextureMixRatio;
out float Diff;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);

void main()
{
    gl_Position = projectionView * model * vec4(pos, 1.0);
    vec3 Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(pos, 1.0));
    TexCoords = texCoords;
    PosHeight = pos.y;
    vec3 LightDir = normalize(-lightDirTo);
    float TransitionRatio = clamp(1.0 + PosHeight * (1 / 0.5), 0.0, 1.0);
    vec3 texNormal = texture(base_normal, vec2(FragPos.x / tilesDimension + 0.5, FragPos.z / tilesDimension + 0.5)).rgb;
    vec3 NormalCorrected = normalize((1.0 - TransitionRatio) * Normal + TransitionRatio * (NORMAL + texNormal));
    TextureMixRatio = texNormal.r;
    Diff = max(dot(NormalCorrected, LightDir), 0.0);
}
