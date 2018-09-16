#version 450
layout (early_fragment_tests) in;

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;
in vec2  v_TexCoords;
in float v_PosHeight;
in float v_TextureMixRatio;
in float v_SpecularComponent;
in vec3  v_ProjectedCoords;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_hills_diffuse;
uniform sampler2D u_hills_diffuse2;
uniform sampler2D u_hills_specular;
uniform sampler2D u_diffuse_mix_map;
uniform float     u_mapDimension;
uniform float     u_maxHillHeight;
uniform sampler2D u_shadowMap;
uniform sampler2D u_normal_map;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;

const float TEXEL_SIZE_MULTIPLIER_OFFSET = 1.0 + clamp((v_PosHeight * 10.0), 0.0, 1.5);
const vec2  TEXEL_SIZE = 0.75 / textureSize(u_shadowMap, 0);
const float SHADOW_INFLUENCE = 0.3;
const float ONE_MINUS_SHADOW_INFLUENCE = 1.0 - SHADOW_INFLUENCE;
const float MAX_DESATURATING_VALUE = 0.8 / ONE_MINUS_SHADOW_INFLUENCE;
const float MIN_CANYON_CIRCLE_HEIGHT = 2.5;
const int   NUM_CANYON_CIRCLES = 12;

float calculateLuminosity(vec3 normal)
{
    float closestDepth = texture(u_shadowMap, v_ProjectedCoords.xy).r;
    float currentDepth = v_ProjectedCoords.z;
    float shadow = 0.0;
    float bias = max(0.0004 * (1.0 - dot(normal, u_lightDir)), 0.000264);   //[0.0006; 0.0004]*0.66;
    vec2 adjustedTexelSize = (TEXEL_SIZE_MULTIPLIER_OFFSET - dot(normal, u_lightDir) * TEXEL_SIZE_MULTIPLIER_OFFSET) * TEXEL_SIZE;

    //PCF filtering
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(u_shadowMap, v_ProjectedCoords.xy + vec2(x,y) * adjustedTexelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 0.066 : 0.0;
        }
    }
    return (1.0 - shadow * SHADOW_INFLUENCE);
}

vec4 desaturate(vec4 fragColor, float desaturatingValue)
{
    float colorMedian = (fragColor.r + fragColor.g + fragColor.b) * 0.333;
    vec4 desaturated = vec4(mix(fragColor.rgb, vec3(colorMedian), desaturatingValue), fragColor.a);
    return desaturated;
}

void createCanyonRings(inout vec4 fragColor, vec3 circleColor, float minHeight, float maxHeight, int numCircles, int circlesRandomMultiplier)
{
    vec2 canyonDistrubution = texture(u_diffuse_mix_map, v_FragPos.xz * u_mapDimension + 0.5).rg;
    float[2] canyonCircleOffset = {canyonDistrubution.r, canyonDistrubution.g};
    float canyonCircleStep = (maxHeight - minHeight) / (numCircles);
    vec3 canyonColor = circleColor * (1.0 - v_Normal.y);
    float influenceStep = 0.1 / (numCircles - 1);
    for (int i = 0; i < numCircles; i++)
    {
        float influence = 1.0 + i * influenceStep;
        float density = 2.0 + influence;
        fragColor.xyz += (influence - clamp(abs(density * (v_FragPos.y
                                                - (minHeight + i * canyonCircleStep))
                                                + canyonCircleOffset[i%2] * circlesRandomMultiplier),
                                            0.0, influence))
                         * canyonColor;
    }
}

void main()
{
    vec4 sampledDiffuse =
        mix(mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), v_TextureMixRatio),
            mix(texture(u_hills_diffuse, v_TexCoords), texture(u_hills_diffuse2, v_TexCoords), v_TextureMixRatio),
            clamp(v_PosHeight, 0.0, 1.0));
    vec4 sampledSpecular =
        mix(vec4(0.0), texture(u_hills_specular, v_TexCoords), clamp(v_PosHeight, 0.0, 1.0));

    vec3 ambientColor = 0.12 * sampledDiffuse.rgb;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 resultColor;

    //swizzle z and y to rotate Z-aligned normal map 90 degrees around X axis, as like we look at it upside down
    //also scale up texture mapping a bit
    vec3 ShadingNormal = texture(u_normal_map, v_FragPos.xz * 0.0625).xzy;
    vec3 ShadingNormalFlat = ShadingNormal;
    ShadingNormalFlat.y *= 0.4;
    ShadingNormalFlat = normalize(ShadingNormalFlat);
    vec3 ShadingNormalHill = ShadingNormal;
    ShadingNormalHill.y *= 0.5;
    ShadingNormalHill = normalize(v_Normal * 4.0 - ShadingNormalHill * 3.2);

    float DiffuseComponentHill = max(dot(ShadingNormalHill, u_lightDir), 0.0) + 0.1;
    float DiffuseComponentFlat = dot(ShadingNormalFlat, u_lightDir);
    float diffuseComponent = mix(DiffuseComponentFlat, DiffuseComponentHill, clamp(v_PosHeight, 0.0, 1.0));

    if (u_shadowEnable)
    {
        float luminosity = calculateLuminosity(ShadingNormalHill);
        diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
        specularColor = v_SpecularComponent * sampledSpecular.rgb;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity - ONE_MINUS_SHADOW_INFLUENCE);
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        specularColor = v_SpecularComponent * sampledSpecular.rgb;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }

    createCanyonRings(o_FragColor, ambientColor + diffuseColor, MIN_CANYON_CIRCLE_HEIGHT, u_maxHillHeight, NUM_CANYON_CIRCLES, 4);
}
