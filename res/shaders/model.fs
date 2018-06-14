#version 450

out vec4 o_FragColor;

in vec2  v_TexCoords;
in vec3  v_Normal;
in float v_DiffuseComponent;
in float v_SpecularComponent;

uniform sampler2D u_texture_diffuse;
uniform sampler2D u_texture_specular;
uniform bool      u_shadow;

void main()
{
    vec4 sampledDiffuse = texture(u_texture_diffuse, v_TexCoords);
    vec4 sampledSpecular = texture(u_texture_specular, v_TexCoords);
    vec3 ambientColor = 0.2 * sampledDiffuse.rgb;
    vec3 diffuseColor = sampledDiffuse.rgb * (v_DiffuseComponent * 0.6 + 0.4);
    vec3 specularColor = v_SpecularComponent * sampledSpecular.rgb;
    vec3 resultColor = ambientColor + diffuseColor + specularColor;

    o_FragColor = vec4(resultColor, sampledDiffuse.a);
    if (u_shadow)
        o_FragColor += clamp(o_FragColor * v_Normal.y / 2, -0.06, 0.03);
}
