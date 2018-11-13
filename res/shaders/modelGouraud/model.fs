#version 450
layout (early_fragment_tests) in;

out vec4 o_FragColor;

in vec2  v_TexCoords;
in vec3  v_Normal;
in float v_DiffuseComponent;
in float v_SpecularComponent;
in vec3  v_ProjectedCoords;
in float v_FlatBlend;

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_specular1;
uniform bool      u_shadow;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
uniform bool      u_useFlatBlending;

const vec2  TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float SHADOW_INFLUENCE = 0.5;
const float MAX_DESATURATING_VALUE = 0.6;

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

float calculateLuminosity(vec3 normal)
{
    float currentDepth = v_ProjectedCoords.z;
    float shadow = 0.0f;
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

void main()
{
    vec4 sampledDiffuse = texture(u_texture_diffuse1, v_TexCoords);
    vec4 sampledSpecular = sampledDiffuse * texture(u_texture_specular1, v_TexCoords).r;
    vec3 ambientColor = 0.08 * sampledDiffuse.rgb;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 resultColor;

    float luminosity = 0.0;
    if (u_shadowEnable)
    {
        luminosity = calculateLuminosity(v_Normal);
        diffuseColor = luminosity * sampledDiffuse.rgb * v_DiffuseComponent;
        specularColor = luminosity * sampledSpecular.rgb * v_SpecularComponent;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity);
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * v_DiffuseComponent;
        specularColor = sampledSpecular.rgb * v_SpecularComponent;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }

    if (u_shadow)
        o_FragColor += clamp(o_FragColor * v_Normal.y * 0.5, -0.05, 0.01);

    //perform flat blending
    if(u_useFlatBlending)
    {
        float flatBlend = clamp(v_FlatBlend, 0.0, 1.0);
        o_FragColor.a = mix(0.0, 1.0, flatBlend);
    }
}
