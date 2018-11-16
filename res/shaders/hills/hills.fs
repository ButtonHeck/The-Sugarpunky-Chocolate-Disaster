#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;
in vec2  v_TexCoords;
in float v_TerrainTypeMix;
in float v_SpecularComponent;
in vec3  v_ProjectedCoords;

uniform sampler2D u_land_diffuse;
uniform sampler2D u_land_diffuse2;
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
uniform bool      u_debugRenderMode;

const vec3  NORMAL = vec3(0.0, 1.0, 0.0);
const vec2  TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float SHADOW_INFLUENCE = 0.5;
const float MAX_DESATURATING_VALUE_HILL = 0.4;
const float MAX_DESATURATING_VALUE_LAND = 0.5;
const float MIN_CANYON_CIRCLE_HEIGHT = 2.5;
const int   NUM_CANYON_CIRCLES = 12;

float SampleShadowMap(sampler2D shadowMap, vec2 coords, float compare)
{
    return step(texture2D(shadowMap, coords.xy).r, compare);
}

float SampleShadowMapLinear(sampler2D shadowMap, vec2 coords, float compare, vec2 texelSize)
{
    vec2 pixelPos = coords/texelSize + vec2(0.5);
    vec2 fracPart = fract(pixelPos);
    vec2 startTexel = (pixelPos - fracPart) * texelSize;

    float blTexel = SampleShadowMap(shadowMap, startTexel, compare);
    float brTexel = SampleShadowMap(shadowMap, startTexel + vec2(texelSize.x, 0.0), compare);
    float tlTexel = SampleShadowMap(shadowMap, startTexel + vec2(0.0, texelSize.y), compare);
    float trTexel = SampleShadowMap(shadowMap, startTexel + texelSize, compare);

    float mixA = mix(blTexel, tlTexel, fracPart.y);
    float mixB = mix(brTexel, trTexel, fracPart.y);

    return mix(mixA, mixB, fracPart.x);
}


float calculateLuminosity()
{
    float currentDepth = v_ProjectedCoords.z;
    float shadow = 0.0;
    float bias = 6.0 / 8192;

    //PCF filtering
    const int NUM_SAMPLES = 3;
    const int SAMPLE_START = (NUM_SAMPLES - 1) / 2;
    const int NUM_SAMPLES_SQUARED = NUM_SAMPLES * NUM_SAMPLES;
    for (int x = -SAMPLE_START; x <= SAMPLE_START; ++x)
    {
        for (int y = -SAMPLE_START; y <= SAMPLE_START; ++y)
        {
            vec2 coordsOffset = vec2(x,y) * TEXEL_SIZE;
            shadow += SampleShadowMapLinear(u_shadowMap, v_ProjectedCoords.xy + coordsOffset, currentDepth - bias, TEXEL_SIZE);
        }
    }

    return 1.0 - (shadow / NUM_SAMPLES_SQUARED * SHADOW_INFLUENCE);
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
    if (u_debugRenderMode)
        o_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
    {
        float DiffuseTextureMix = texture(u_diffuse_mix_map, v_FragPos.xz * u_mapDimension + 0.5).r;
        float TerrainTypeMixClamped = clamp(v_TerrainTypeMix, 0.0, 1.0);
        vec4 sampledDiffuse = mix(mix(texture(u_land_diffuse, v_TexCoords * 2.0),
                                      texture(u_land_diffuse2, v_TexCoords * 2.0),
                                      DiffuseTextureMix),
                                  mix(texture(u_hills_diffuse, v_TexCoords),
                                      texture(u_hills_diffuse2, v_TexCoords),
                                      DiffuseTextureMix),
                                  TerrainTypeMixClamped);
        vec4 sampledSpecular = mix(vec4(0.0), texture(u_hills_specular, v_TexCoords), TerrainTypeMixClamped);

        vec3 ambientColor = 0.08 * sampledDiffuse.rgb;
        vec3 diffuseColor;
        vec3 specularColor;
        vec3 resultColor;

        vec3 ShadingNormal = (texture(u_normal_map, v_FragPos.xz * 0.125).xyz) * 2.0 - 0.66;
        vec3 ShadingNormalLand = normalize(NORMAL + 0.6 * ShadingNormal);
        vec3 ShadingNormalHill = ShadingNormal;
        ShadingNormalHill = normalize(v_Normal + 0.6 * ShadingNormalHill);

        float DiffuseComponentHill = max(dot(ShadingNormalHill, u_lightDir), 0.0);
        float DiffuseComponentLand = max(dot(ShadingNormalLand, u_lightDir), 0.0);
        float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));
        float diffuseComponent = mix(DiffuseComponentLand, DiffuseComponentHill, TerrainTypeMixClamped) * sunPositionAttenuation;

        if (u_shadowEnable)
        {
            float luminosity = calculateLuminosity();
            diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
            specularColor = v_SpecularComponent * sampledSpecular.rgb * sunPositionAttenuation;
            resultColor = ambientColor + diffuseColor + specularColor;
            o_FragColor = vec4(resultColor, sampledDiffuse.a);
            float desaturatingValue = mix(0.0,
                                          mix(MAX_DESATURATING_VALUE_LAND, MAX_DESATURATING_VALUE_HILL, TerrainTypeMixClamped),
                                          min(luminosity, diffuseComponent + SHADOW_INFLUENCE));
            o_FragColor = desaturate(o_FragColor, desaturatingValue);
        }
        else
        {
            diffuseColor = sampledDiffuse.rgb * diffuseComponent;
            specularColor = v_SpecularComponent * sampledSpecular.rgb * sunPositionAttenuation;
            resultColor = ambientColor + diffuseColor + specularColor;
            o_FragColor = vec4(resultColor, sampledDiffuse.a);
        }

        createCanyonRings(o_FragColor, ambientColor + diffuseColor, MIN_CANYON_CIRCLE_HEIGHT, u_maxHillHeight, NUM_CANYON_CIRCLES, 4);
    }
}
