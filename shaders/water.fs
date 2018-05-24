#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 LightDir;

uniform sampler2D water_diffuse;
uniform sampler2D water_specular;
uniform samplerCube skybox;
uniform vec3 viewPosition;

void main()
{
    vec3 I = normalize(FragPos - viewPosition);
    vec3 normal = normalize(Normal);
    vec3 R = reflect(I, normalize(0.5 * normal + vec3(0.0, 1.0, 0.0)));
    vec4 skyFragColor = vec4(texture(skybox, R).rgb, 1.0);

    vec3 viewDir = normalize(viewPosition - FragPos);
    vec4 sampledDiffuse = texture(water_diffuse, TexCoords);
    vec4 sampledSpecular = texture(water_specular, TexCoords);
    //diffuse shading
    float diff = max(dot(normal, LightDir), 0.0);
    //specular shading
    vec3 reflect = reflect(-LightDir, normal);
    float spec = pow(max(dot(reflect, viewDir), 0.0), 32.0);

    vec3 diffuse = diff * sampledDiffuse.rgb * 0.4 + 0.6 * sampledDiffuse.rgb;
    vec3 specular = spec * sampledSpecular.rgb * 3;
    vec3 result = diffuse + specular;

    FragColor = vec4(0.5 * result + 0.5 * skyFragColor.rgb, sampledDiffuse.a);
}
