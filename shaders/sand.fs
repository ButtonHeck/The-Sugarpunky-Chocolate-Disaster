#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float PosHeight;
in vec3 LightDir;

uniform sampler2D base_diffuse;
uniform sampler2D base_diffuse2;
uniform sampler2D base_specular;
uniform sampler2D base_normal;
uniform sampler2D sand_diffuse;
uniform sampler2D sand_diffuse2;
uniform sampler2D sand_specular;
uniform vec3 viewPosition;
uniform float waterLevel;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);

void main()
{
    vec3 texNormal = texture(base_normal, vec2(FragPos.x / 768, FragPos.z / 768)).rgb;
    float transitionRatio = clamp(1.0 + PosHeight * (1 / 0.5), 0.0, 1.0);
    vec3 normal = normalize((1.0 - transitionRatio) * Normal + transitionRatio * (NORMAL + texNormal));
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec4 sampledDiffuse =
                mix(mix(texture(sand_diffuse, TexCoords), texture(sand_diffuse2, TexCoords), texNormal.r),
                    mix(texture(base_diffuse, TexCoords), texture(base_diffuse2, TexCoords), texNormal.r),
                    max(min(PosHeight * 2.1 - waterLevel + (1.0 + waterLevel), 1.0), 0.0));
    vec4 sampledSpecular =
                mix(texture(sand_specular, TexCoords),
                    texture(base_specular, TexCoords),
                    max(min(PosHeight * 2.1 - waterLevel + (1.0 + waterLevel), 1.0), 0.0));
    //diffuse shading
    float diff = max(dot(normal, LightDir), 0.0);
    //specular shading
    vec3 reflect = reflect(-LightDir, normal);
    float spec = pow(max(dot(reflect, viewDir), 0.0), 64.0);

    vec3 diffuse = diff * sampledDiffuse.rgb * 0.33 + 0.67 * sampledDiffuse.rgb;
    vec3 specular = spec * sampledSpecular.rgb;
    vec3 result = diffuse + specular;

    FragColor = vec4(result, sampledDiffuse.a);
}
