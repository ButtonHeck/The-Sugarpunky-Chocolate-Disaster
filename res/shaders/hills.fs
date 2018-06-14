#version 450

out vec4 o_FragColor;

in vec2  vg_TexCoords;
in float vg_PosHeight;
in float vg_TextureFlatMixRatio;
in float vg_TextureHillMixRatio;
in float vg_DiffuseComponentHill;
in float vg_DiffuseComponentFlat;
in float vg_SpecularComponent;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_hills_diffuse;
uniform sampler2D u_hills_diffuse2;
uniform sampler2D u_hills_specular;

void main()
{
    vec4 sampledDiffuse =
        mix(mix(texture(u_flat_diffuse, vg_TexCoords), texture(u_flat_diffuse2, vg_TexCoords), vg_TextureFlatMixRatio),
            mix(texture(u_hills_diffuse, vg_TexCoords), texture(u_hills_diffuse2, vg_TexCoords), vg_TextureHillMixRatio),
            min(max(0.0, vg_PosHeight), 1.0));
    vec4 sampledSpecular =
        mix(vec4(0.0), texture(u_hills_specular, vg_TexCoords), min(max(0.0, vg_PosHeight), 1.0));

    vec3 diffuseColor = sampledDiffuse.rgb * (mix(vg_DiffuseComponentFlat, vg_DiffuseComponentHill, min(max(0.0, vg_PosHeight), 1.0)) * 0.5 + 0.5);
    vec3 specularColor = vg_SpecularComponent * sampledSpecular.rgb;
    vec3 resultColor = diffuseColor + specularColor;

    o_FragColor = vec4(resultColor, sampledDiffuse.a);
}
