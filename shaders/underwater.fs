#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in float Diff;

uniform sampler2D underwater_diffuse;
uniform sampler2D bottomRelief;
uniform int tilesDimension;

void main()
{
    float reliefTexture = texture(bottomRelief, vec2(FragPos.x / tilesDimension + 0.5, FragPos.z / tilesDimension + 0.5)).r * 1.1;
    vec4 sampledDiffuse = texture(underwater_diffuse, TexCoords);
    vec3 diffuse = Diff * sampledDiffuse.rgb * (1.0 - reliefTexture);
    FragColor = vec4(diffuse, sampledDiffuse.a);
}
