#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float PosHeight;
in vec3 LightDir;

uniform sampler2D base_diffuse;
uniform sampler2D base_diffuse2;
uniform sampler2D base_normal;
uniform sampler2D sand_diffuse;
uniform sampler2D sand_diffuse2;
uniform vec3 viewPosition;
uniform float waterLevel;
uniform int tilesDimension;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);

void main()
{
    vec3 texNormal = texture(base_normal, vec2(FragPos.x / tilesDimension + 0.5, FragPos.z / tilesDimension + 0.5)).rgb;
    float transitionRatio = clamp(1.0 + PosHeight * (1 / 0.5), 0.0, 1.0);
    vec3 normal = normalize((1.0 - transitionRatio) * Normal + transitionRatio * (NORMAL + texNormal));
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec4 sampledDiffuse =
                mix(mix(texture(sand_diffuse, TexCoords), texture(sand_diffuse2, TexCoords), texNormal.r),
                    mix(texture(base_diffuse, TexCoords), texture(base_diffuse2, TexCoords), texNormal.r),
                    max(min(PosHeight * 2.1 - waterLevel + (1.0 + waterLevel), 1.0), 0.0));
    //diffuse shading
    float diff = max(dot(normal, LightDir), 0.0);

    vec3 diffuse = (diff * sampledDiffuse.rgb * 0.4 + 0.6 * sampledDiffuse.rgb) * (1 + FragPos.y / 2.5);
    vec3 result = diffuse;

    FragColor = vec4(result, sampledDiffuse.a);
}
