#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D water_diffuse;
uniform sampler2D water_specular;
uniform samplerCube skybox;
uniform vec3 viewPosition;
uniform vec3 lightDirTo;

void main()
{
    vec3 I = normalize(FragPos - viewPosition);
    vec3 normal = normalize(Normal);
    vec3 R = reflect(I, normalize(0.5 * normal + vec3(0.0, 1.0, 0.0)));
    vec4 skyFragColor = vec4(texture(skybox, R).rgb, 1.0);

    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 lightDir = normalize(-lightDirTo);
    vec4 sampledDiffuse = texture(water_diffuse, TexCoords);
    vec4 sampledSpecular = texture(water_specular, TexCoords);
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0) * 0.5;
    //specular shading
    vec3 reflect = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflect, viewDir), 0.0), 32.0);

    vec3 diffuse = diff * sampledDiffuse.rgb + 0.5 * sampledDiffuse.rgb;
    vec3 specular = spec * sampledSpecular.rgb;
    vec3 result = diffuse + specular;

    FragColor = vec4(0.5 * result + 0.5 * skyFragColor.rgb, sampledDiffuse.a);
}
