#version 450

out vec4 FragColor;

in vec2 TexCoords;
in float PosHeight;
in float TextureMixRatio;
in float Diff;
in float DiffFlat;
in float Spec;

uniform sampler2D base_diffuse;
uniform sampler2D base_diffuse2;
uniform sampler2D hills_diffuse;
uniform sampler2D hills_diffuse2;
uniform sampler2D hills_specular;

void main()
{
    vec4 sampledDiffuse =
        mix(mix(texture(base_diffuse, TexCoords), texture(base_diffuse2, TexCoords), TextureMixRatio),
            mix(texture(hills_diffuse, TexCoords), texture(hills_diffuse2, TexCoords), TextureMixRatio * 1.5),
            min(max(0.0, PosHeight/1.5), 1.0));
    vec4 sampledSpecular =
        mix(vec4(0.0), texture(hills_specular, TexCoords), min(max(0.0, PosHeight/1.5), 1.0));

    vec3 diffuse = mix(DiffFlat, Diff, min(max(0.0, PosHeight/1.5), 1.0)) * sampledDiffuse.rgb * 0.4 + 0.6 * sampledDiffuse.rgb;
    vec3 specular = Spec * sampledSpecular.rgb;
    vec3 result = diffuse + specular;

    FragColor = vec4(result, sampledDiffuse.a);
}
