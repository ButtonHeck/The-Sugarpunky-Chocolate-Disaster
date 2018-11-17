#version 450

out vec4 o_FragColor;

in vec3 v_FragPos;
in vec2 v_TexCoords;
in vec3 v_ProjectedCoords;

uniform sampler2D u_land_diffuse;
uniform sampler2D u_land_diffuse2;
uniform sampler2D u_diffuse_mix_map;
uniform sampler2D u_normal_map;
uniform vec3      u_lightDir;
uniform float     u_mapDimension;
uniform sampler2D u_shadowMap;
uniform bool      u_shadowEnable;

const vec3  NORMAL = vec3(0.0, 1.0, 0.0);
const vec2  TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float SHADOW_INFLUENCE = 0.5;
const float MAX_DESATURATING_VALUE = 0.5;

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
    float shadow = 0.0f;
    float bias = 4.0 / 8192;

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

void main()
{
    float DiffuseTextureMix = texture(u_diffuse_mix_map, v_FragPos.xz * u_mapDimension + 0.5).r;
    vec4 sampledDiffuse = mix(texture(u_land_diffuse, v_TexCoords),
                              texture(u_land_diffuse2, v_TexCoords),
                              DiffuseTextureMix);

    vec3 ambientColor = 0.08 * sampledDiffuse.rgb;
    vec3 diffuseColor;
    vec3 resultColor;

    vec3 ShadingNormal = clamp((texture(u_normal_map, v_FragPos.xz * 0.125).xzy) * 2.2, vec3(0.0), vec3(1.0));
    ShadingNormal.xyz -= vec3(0.5);
    ShadingNormal = normalize(NORMAL + 1.5 * ShadingNormal);

    float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));
    float diffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0) * sunPositionAttenuation;

    if (u_shadowEnable)
    {
        float luminosity = calculateLuminosity();
        diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
        resultColor = ambientColor + diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(luminosity, diffuseComponent + SHADOW_INFLUENCE));
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        resultColor = ambientColor + diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }
}
