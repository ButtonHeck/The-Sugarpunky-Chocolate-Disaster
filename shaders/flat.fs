#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 LightDir;

uniform sampler2D base_diffuse;
uniform sampler2D base_diffuse2;
uniform sampler2D base_normal;
uniform int tilesDimension;

void main()
{
    vec3 texNormal = texture(base_normal, vec2(FragPos.x / tilesDimension + 0.5, FragPos.z / tilesDimension + 0.5)).rgb;
    vec3 normal = normalize(vec3(0.0, 1.0, 0.0) + texNormal);
    vec4 sampledDiffuse = mix(texture(base_diffuse, TexCoords), texture(base_diffuse2, TexCoords), texNormal.r);
    //diffuse shading
    float diff = max(dot(normal, LightDir), 0.0);

    vec3 diffuse = diff * sampledDiffuse.rgb * 0.4 + 0.6 * sampledDiffuse.rgb;
    vec3 result = diffuse;

    FragColor = vec4(result, sampledDiffuse.a);
}
