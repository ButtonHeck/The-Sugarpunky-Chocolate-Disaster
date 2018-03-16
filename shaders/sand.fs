#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float PosHeight;

uniform sampler2D base_diffuse;
uniform sampler2D base_specular;
uniform sampler2D sand_diffuse;
uniform sampler2D sand_specular;
uniform vec3 viewPosition;
uniform vec3 lightDirTo;
uniform float waterLevel;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 lightDir = normalize(-lightDirTo);
    vec4 sampledDiffuse =
                mix(texture(sand_diffuse, TexCoords),
                    texture(base_diffuse, TexCoords),
                    max(min(PosHeight * 2.1 - waterLevel + (1.0 + waterLevel), 1.0), 0.0));
    vec4 sampledSpecular =
                mix(texture(sand_specular, TexCoords),
                    texture(base_specular, TexCoords),
                    max(min(PosHeight * 2.1 - waterLevel + (1.0 + waterLevel), 1.0), 0.0));
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //specular shading
    vec3 reflect = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflect, viewDir), 0.0), 64.0);

    vec3 diffuse = diff * sampledDiffuse.rgb * 0.3 + 0.7 * sampledDiffuse.rgb;
    vec3 specular = spec * sampledSpecular.rgb;
    vec3 result = diffuse + specular;

    FragColor = vec4(result, sampledDiffuse.a);
}
