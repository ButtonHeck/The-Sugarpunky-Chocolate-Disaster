#version 450

out vec4 o_FragColor;

in vec3 v_FragPos;
in vec2 v_TexCoords;
in vec3 v_LightDir;
in vec3 v_ProjectedCoords;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_normal_map;
uniform float     u_mapDimension;
uniform sampler2D u_shadowMap;
uniform bool      u_shadowEnable;

const vec2 TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float DIFFUSE_MIX = 0.5;
const vec3 NORMAL = vec3(0.0, 1.0, 0.0);
const float SHADOW_INFLUENCE = 0.3;
const float SHADOW_BIAS = 0.00025;
const float MAX_DESATURATING_VALUE = 0.8 / (1.0 - SHADOW_INFLUENCE);
const vec2 POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float calculateLuminosity()
{
    float closestDepth = texture(u_shadowMap, v_ProjectedCoords.xy).r;
    float currentDepth = v_ProjectedCoords.z;
    float shadow = 0.0f;

    //poisson filtering
    for (int i = 0; i < 4; i++)
    {
        float poissonDiskDepth = texture(u_shadowMap, v_ProjectedCoords.xy + POISSON_DISK[i] * TEXEL_SIZE).r;
        shadow += currentDepth - SHADOW_BIAS > poissonDiskDepth ? 0.25 : 0.0;
    }
    return (1.0 - shadow * SHADOW_INFLUENCE);
}

vec4 desaturate(vec4 fragColor, float desaturatingValue)
{
    float colorMedian = (fragColor.r + fragColor.g + fragColor.b) * 0.333;
    vec3 gray = vec3(colorMedian);
    vec4 desaturated = vec4(mix(fragColor.rgb, gray, desaturatingValue), fragColor.a);
    return desaturated;
}

void main()
{
    vec3 FlatNormal = texture(u_normal_map, v_FragPos.xz * u_mapDimension + 0.5).rgb;
    vec4 sampledDiffuse = mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), FlatNormal.r);
    vec3 ShadingNormal = normalize(NORMAL + FlatNormal);

    vec3 diffuseColor;
    vec3 resultColor;
    float diffuseComponent = max(dot(ShadingNormal, v_LightDir), 0.0);

    if (u_shadowEnable)
    {
        float luminosity = calculateLuminosity();
        diffuseColor = luminosity * mix(sampledDiffuse.rgb, sampledDiffuse.rgb * diffuseComponent, DIFFUSE_MIX);
        resultColor = diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity - (1.0 - SHADOW_INFLUENCE));
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = mix(sampledDiffuse.rgb, sampledDiffuse.rgb * diffuseComponent, DIFFUSE_MIX);
        resultColor = diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }
}
