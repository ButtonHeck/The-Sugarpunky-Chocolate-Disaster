#version 450

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in float Diff;
in float Spec;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform bool shadow;

void main()
{
    vec4 sampledDiffuse = texture(texture_diffuse, TexCoords);
    vec4 sampledSpecular = texture(texture_specular, TexCoords);
    vec3 ambient = 0.2 * sampledDiffuse.rgb;
    vec3 diffuse = Diff * sampledDiffuse.rgb * 0.5 + 0.5 * sampledDiffuse.rgb;
    vec3 specular = Spec * sampledSpecular.rgb;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, sampledDiffuse.a);
    if (shadow)
        FragColor += clamp(FragColor * Normal.y / 2, -0.06, 0.03);
}
