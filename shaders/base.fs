#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in float PosHeight;

uniform sampler2D base_diffuse;
uniform sampler2D base_specular;
uniform vec3 viewPosition;
uniform vec3 lightDirTo;

void main()
{
    vec3 normal = vec3(0.0, 1.0, 0.0);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 lightDir = normalize(-lightDirTo);
    vec4 sampledDiffuse = texture(base_diffuse, TexCoords);
    vec4 sampledSpecular = texture(base_specular, TexCoords);
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //specular shading
    vec3 reflect = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflect, viewDir), 0.0), 64.0);

    vec3 diffuse = diff * sampledDiffuse.rgb * 0.33 + 0.67 * sampledDiffuse.rgb;
    vec3 specular = spec * sampledSpecular.rgb;
    vec3 result = diffuse + specular;

    FragColor = vec4(result, sampledDiffuse.a);
}
