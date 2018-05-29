#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in float PosHeight;
in float TextureMixRatio;
in float Diff;

uniform sampler2D base_diffuse;
uniform sampler2D base_diffuse2;
uniform sampler2D sand_diffuse;
uniform sampler2D sand_diffuse2;
uniform float waterLevel;

void main()
{
    vec4 sampledDiffuse =
                mix(mix(texture(sand_diffuse, TexCoords), texture(sand_diffuse2, TexCoords), TextureMixRatio),
                    mix(texture(base_diffuse, TexCoords), texture(base_diffuse2, TexCoords), TextureMixRatio),
                    max(min(PosHeight * 2.1 - waterLevel + (1.0 + waterLevel), 1.0), 0.0));
    vec3 diffuse = (Diff * sampledDiffuse.rgb * 0.4 + 0.6 * sampledDiffuse.rgb) * (1 + FragPos.y / 2.5);
    vec3 result = diffuse;

    FragColor = vec4(result, sampledDiffuse.a);
}
