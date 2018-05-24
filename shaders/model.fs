#version 450

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform vec3 viewPosition;
uniform vec3 lightDirTo;
uniform bool shadow;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 lightDir = normalize(-lightDirTo);
    vec4 sampledDiffuse = texture(texture_diffuse, vec2(TexCoords.x, TexCoords.y));
    vec4 sampledSpecular = texture(texture_specular, TexCoords);
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //specular shading
    vec3 reflect = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflect, viewDir), 0.0), 8.0) * 0.33;

    vec3 ambient = 0.2 * sampledDiffuse.rgb;
    vec3 diffuse = diff * sampledDiffuse.rgb * 0.5 + 0.5 * sampledDiffuse.rgb;
    vec3 specular = spec * sampledSpecular.rgb;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, sampledDiffuse.a);
    if (shadow)
        FragColor += clamp(FragColor * Normal.y / 4, -0.06, 0.03);
}
