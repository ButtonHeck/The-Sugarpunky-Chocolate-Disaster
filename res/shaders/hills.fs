#version 450

out vec4 o_FragColor;

in vec2  vg_TexCoords;
in float vg_PosHeight;
in float vg_TextureFlatMixRatio;
in float vg_TextureHillMixRatio;
in float vg_DiffuseComponentHill;
in float vg_DiffuseComponentFlat;
in float vg_SpecularComponent;
in vec4  vg_FragPosLightSpace;
in vec3  vg_Normal;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_hills_diffuse;
uniform sampler2D u_hills_diffuse2;
uniform sampler2D u_hills_specular;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;

float calculateShadowComponent(vec4 fragPosLightSpace, vec3 normal)
{
    //transform from [-1;1] to [0;1]
    vec3 projCoords = fragPosLightSpace.xyz * 0.5 + 0.5;
    float closestDepth = texture(u_shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    float bias = max(0.0006 * (1.0 - dot(normal, u_lightDir)), 0.0004);
    vec2 texelSize = (2.5 - dot(normal, u_lightDir) * 2.5) / textureSize(u_shadowMap, 0);

    //PCF filtering
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth - bias * 0.66 > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 15.0;

    if (projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    vec4 sampledDiffuse =
        mix(mix(texture(u_flat_diffuse, vg_TexCoords), texture(u_flat_diffuse2, vg_TexCoords), vg_TextureFlatMixRatio),
            mix(texture(u_hills_diffuse, vg_TexCoords), texture(u_hills_diffuse2, vg_TexCoords), vg_TextureHillMixRatio),
            min(max(0.0, vg_PosHeight), 1.0));
    vec4 sampledSpecular =
        mix(vec4(0.0), texture(u_hills_specular, vg_TexCoords), min(max(0.0, vg_PosHeight), 1.0));

    vec3 diffuseColor;
    if (u_shadowEnable)
    {
        float shadowComponent = calculateShadowComponent(vg_FragPosLightSpace, vg_Normal);
        diffuseColor = (1.0 - shadowComponent * 0.3) * sampledDiffuse.rgb *
            (mix(vg_DiffuseComponentFlat, vg_DiffuseComponentHill, min(max(0.0, vg_PosHeight), 1.0)) * 0.5 + 0.5);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb *
            (mix(vg_DiffuseComponentFlat, vg_DiffuseComponentHill, min(max(0.0, vg_PosHeight), 1.0)) * 0.5 + 0.5);
    }

    vec3 specularColor = vg_SpecularComponent * sampledSpecular.rgb;
    vec3 resultColor = diffuseColor + specularColor;

    o_FragColor = vec4(resultColor, sampledDiffuse.a);
}
