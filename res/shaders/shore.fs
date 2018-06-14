#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec2  v_TexCoords;
in float v_PosHeight;
in float v_TextureMixRatio;
in float v_DiffuseComponent;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_sand_diffuse;
uniform sampler2D u_sand_diffuse2;

void main()
{
    vec4 sampledDiffuse =
                mix(mix(texture(u_sand_diffuse, v_TexCoords), texture(u_sand_diffuse2, v_TexCoords), v_TextureMixRatio),
                    mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), v_TextureMixRatio),
                    max(min(v_PosHeight + 1.0, 1.0), 0.0));
    vec3 diffuseColor = sampledDiffuse.rgb * (v_DiffuseComponent * 0.5 + 0.5) * (1 + v_FragPos.y / 2.5);
    vec3 resultColor = diffuseColor;

    o_FragColor = vec4(resultColor, sampledDiffuse.a);
}
