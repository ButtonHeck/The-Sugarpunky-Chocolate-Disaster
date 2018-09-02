#version 450

out vec4 o_FragColor;

in vec3 v_FragPos;
in vec2 v_TexCoords;
in vec3 v_ProjectedCoords;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform vec3      u_lightDir;
uniform sampler2D u_normal_map;
uniform float     u_mapDimension;
uniform sampler2D u_shadowMap;
uniform bool      u_shadowEnable;

const vec2 TEXEL_SIZE = 0.75 / textureSize(u_shadowMap, 0);
const float DIFFUSE_MIX = 0.7;
const vec3 NORMAL = vec3(0.0, 1.0, 0.0);
const float SHADOW_INFLUENCE = 0.3;
const float ONE_MINUS_SHADOW_INFLUENCE = 1.0 - SHADOW_INFLUENCE;
const float SHADOW_BIAS = 0.00025;
const float MAX_DESATURATING_VALUE = 0.8 / ONE_MINUS_SHADOW_INFLUENCE;
const vec2 POISSON_DISK[9] = vec2[](
    vec2(0.95581, -0.27159), vec2(0.50147, -0.51807), vec2(0.69607, 0.51559),
    vec2(-0.003682, -0.9015), vec2(0.1593, 0.13975), vec2(-0.6503, 0.0918),
    vec2(0.11915, 0.95449), vec2(-0.34296, 0.75575), vec2(-0.6038, -0.6527)
);

float calculateLuminosity()
{
    float closestDepth = texture(u_shadowMap, v_ProjectedCoords.xy).r;
    float currentDepth = v_ProjectedCoords.z;
    float shadow = 0.0f;

    //poisson filtering
    for (int i = 0; i < 9; i++)
    {
        float poissonDiskDepth = texture(u_shadowMap, v_ProjectedCoords.xy + POISSON_DISK[i] * TEXEL_SIZE).r;
        float distanceEffect = max(0.21 - (45.0 * (currentDepth - poissonDiskDepth)), 0.0);
        shadow += currentDepth - SHADOW_BIAS > poissonDiskDepth ? (0.111 + 0.125 * distanceEffect) : 0.0;
    }
    return (1.0 - shadow * SHADOW_INFLUENCE);
}

vec4 desaturate(vec4 fragColor, float desaturatingValue)
{
    float colorMedian = (fragColor.r + fragColor.g + fragColor.b) * 0.333;
    vec4 desaturated = vec4(mix(fragColor.rgb, vec3(colorMedian), desaturatingValue), fragColor.a);
    return desaturated;
}

void main()
{
    vec3 FlatNormal = texture(u_normal_map, v_FragPos.xz * u_mapDimension + 0.5).rgb;
    vec4 sampledDiffuse = mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), FlatNormal.r * 1.25);
    vec3 ShadingNormal = normalize(NORMAL + FlatNormal);

    vec3 diffuseColor;
    vec3 resultColor;
    float diffuseComponent = dot(ShadingNormal, u_lightDir);

    if (u_shadowEnable)
    {
        float luminosity = calculateLuminosity();
        diffuseColor = luminosity * mix(sampledDiffuse.rgb, sampledDiffuse.rgb * diffuseComponent, DIFFUSE_MIX);
        resultColor = diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity - ONE_MINUS_SHADOW_INFLUENCE);
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = mix(sampledDiffuse.rgb, sampledDiffuse.rgb * diffuseComponent, DIFFUSE_MIX);
        resultColor = diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }
}
