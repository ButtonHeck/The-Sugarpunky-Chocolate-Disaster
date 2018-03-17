#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D base_diffuse;
uniform sampler2D base_specular;
uniform sampler2D hills_diffuse;
uniform sampler2D hills_specular;
uniform vec3 viewPosition;
uniform vec3 lightDirTo;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);

void main()
{
    float PosHeight = FragPos.y;
    float transitionRatio = clamp(0.0 + PosHeight * (1 / 0.66), 0.0, 1.0);
    vec3 normal = normalize(transitionRatio * Normal + (1.0 - transitionRatio) * NORMAL);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 lightDir = normalize(-lightDirTo);
    vec4 sampledDiffuse =
        mix(texture(base_diffuse, TexCoords), texture(hills_diffuse, TexCoords), min(max(0.0, PosHeight/1.5), 1.0));
    vec4 sampledSpecular =
        mix(texture(base_specular, TexCoords), texture(hills_specular, TexCoords), min(max(0.0, PosHeight/1.5), 1.0));
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
