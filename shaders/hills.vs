#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 projectionView;
uniform vec3 lightDirTo;
uniform int tilesDimension;
uniform sampler2D base_normal;
uniform vec3 viewPosition;

out vec2 TexCoords;
out float PosHeight;
out float Diff;
out float DiffFlat;
out float TextureMixRatio;
out float Spec;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);

void main()
{
    gl_Position = projectionView * model * vec4(pos, 1.0);
    vec3 Normal = mat3(transpose(inverse(model))) * normal;
    vec3 FragPos = vec3(model * vec4(pos, 1.0));
    TexCoords = texCoords;
    PosHeight = pos.y;
    vec3 LightDir = normalize(-lightDirTo);
    vec3 TexNormal = texture(base_normal, vec2(FragPos.x / tilesDimension + 0.5, FragPos.z / tilesDimension + 0.5)).rgb;
    TextureMixRatio = TexNormal.r;
    float TransitionRatio = clamp(0.0 + PosHeight * (1 / 0.66), 0.0, 1.0);
    vec3 NormalCorrected = normalize(TransitionRatio * Normal + (1.0 - TransitionRatio) * TexNormal);
    Diff = max(dot(NormalCorrected, LightDir), 0.0);
    DiffFlat = max(dot(normalize(NORMAL + TexNormal), LightDir), 0.0);
    vec3 Reflect = reflect(-LightDir, NormalCorrected);
    vec3 ViewDir = normalize(viewPosition - FragPos);
    Spec = pow(max(dot(Reflect, ViewDir), 0.0), 64.0);
}
