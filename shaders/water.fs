#version 450

out vec4 FragColor;

in vec2 TexCoords;
in vec3 R;
in float Diff;
in float Spec;

uniform sampler2D water_diffuse;
uniform sampler2D water_specular;
uniform samplerCube skybox;

void main()
{
    vec4 skyFragColor = vec4(texture(skybox, R).rgb, 1.0);
    vec4 sampledDiffuse = texture(water_diffuse, TexCoords);
    vec4 sampledSpecular = texture(water_specular, TexCoords);

    vec3 diffuse = Diff * sampledDiffuse.rgb * 0.4 + 0.6 * sampledDiffuse.rgb;
    vec3 specular = Spec * sampledSpecular.rgb * 3;
    vec3 result = diffuse + specular;

    FragColor = vec4(0.5 * result + 0.5 * skyFragColor.rgb, sampledDiffuse.a);
}
