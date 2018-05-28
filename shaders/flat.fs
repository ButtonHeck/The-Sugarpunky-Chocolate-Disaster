#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in float Diff;

uniform sampler2D base_diffuse;
uniform sampler2D base_diffuse2;
uniform sampler2D base_normal;
uniform int tilesDimension;

void main()
{
    vec4 sampledDiffuse = mix(texture(base_diffuse, TexCoords), texture(base_diffuse2, TexCoords),
                              texture(base_normal, vec2(FragPos.x / tilesDimension + 0.5, FragPos.z / tilesDimension + 0.5)).r);
    //diffuse shading
    vec3 diffuse = Diff * sampledDiffuse.rgb * 0.4 + 0.6 * sampledDiffuse.rgb;
    vec3 result = diffuse;

    FragColor = vec4(result, sampledDiffuse.a);
}
