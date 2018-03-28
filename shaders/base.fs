#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in float PosHeight;
in vec3 LightDir;

uniform sampler2D base_diffuse;
uniform sampler2D base_diffuse2;
uniform sampler2D base_specular;
uniform sampler2D base_normal;
uniform vec3 viewPosition;

void main()
{
    vec3 texNormal = texture(base_normal, vec2(FragPos.x / 768, FragPos.z / 768)).rgb;
    vec3 normal = normalize(vec3(0.0, 1.0, 0.0) + texNormal);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec4 sampledDiffuse = mix(texture(base_diffuse, TexCoords), texture(base_diffuse2, TexCoords), texNormal.r);
    vec4 sampledSpecular = texture(base_specular, TexCoords);
    //diffuse shading
    float diff = max(dot(normal, LightDir), 0.0);
    //specular shading
    vec3 reflect = reflect(-LightDir, normal);
    float spec = pow(max(dot(reflect, viewDir), 0.0), 64.0);

    vec3 diffuse = diff * sampledDiffuse.rgb * 0.33 + 0.67 * sampledDiffuse.rgb;
    vec3 specular = spec * sampledSpecular.rgb;
    vec3 result = diffuse + specular;

    FragColor = vec4(result, sampledDiffuse.a);
}
