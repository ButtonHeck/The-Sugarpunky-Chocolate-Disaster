#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 LightDir;

uniform sampler2D underwater_diffuse;
uniform sampler2D underwater_normal;
uniform sampler2D bottomRelief;
uniform vec3 viewPosition;
uniform int tilesDimension;

void main()
{
    float reliefTexture = texture(bottomRelief, vec2(FragPos.x / tilesDimension + 0.5, FragPos.z / tilesDimension + 0.5)).r * 1.1;
    vec3 texNormal = texture(underwater_normal, vec2(FragPos.x / 192, FragPos.z / 192)).rgb;
    vec3 normal = normalize(texNormal);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec4 sampledDiffuse = texture(underwater_diffuse, TexCoords);
    float diff = max(dot(normal, LightDir), 0.0);
    vec3 diffuse = diff * sampledDiffuse.rgb * (1.0 - reliefTexture);
    FragColor = vec4(diffuse, sampledDiffuse.a);
}
