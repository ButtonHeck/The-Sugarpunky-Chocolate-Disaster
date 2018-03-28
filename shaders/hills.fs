#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float PosHeight;
in vec3 LightDir;

uniform sampler2D base_diffuse;
uniform sampler2D base_diffuse2;
uniform sampler2D base_specular;
uniform sampler2D base_normal;
uniform sampler2D hills_diffuse;
uniform sampler2D hills_diffuse2;
uniform sampler2D hills_specular;
uniform vec3 viewPosition;
uniform vec3 lightDirTo;

void main()
{
    vec3 texNormal = texture(base_normal, vec2(FragPos.x / 768, FragPos.z / 768)).rgb;
    float transitionRatio = clamp(0.0 + PosHeight * (1 / 0.66), 0.0, 1.0);
    vec3 normal = normalize(transitionRatio * Normal + (1.0 - transitionRatio) * texNormal);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec4 sampledDiffuse =
        mix(mix(texture(base_diffuse, TexCoords), texture(base_diffuse2, TexCoords), texNormal.r),
            mix(texture(hills_diffuse, TexCoords), texture(hills_diffuse2, TexCoords), texNormal.r * 1.5),
            min(max(0.0, PosHeight/1.5), 1.0));
    vec4 sampledSpecular =
        mix(texture(base_specular, TexCoords), texture(hills_specular, TexCoords), min(max(0.0, PosHeight/1.5), 1.0));
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
