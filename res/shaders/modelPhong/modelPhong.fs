#version 450

out vec4 o_FragColor;

in vec2  v_TexCoords;
in vec3  v_Normal;
in vec3  v_ProjectedCoords;
in float v_AlphaValue;
in vec3  v_FragPos;

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_specular1;
uniform vec3      u_viewPosition;
uniform bool      u_shadow;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
uniform bool      u_useLandBlending;
uniform bool      u_isGrass;

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
    float bias = 3.0 / 8192;

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

    vec3 ambientColorDaySelf = 0.12 * sampledDiffuse.rgb;
    vec3 ambientColorNightSelf = 0.03 * sampledDiffuse.rgb;
    vec3 nightAmbientColor = vec3(0.0034, 0.0012, 0.0009);
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 resultColor;

    vec3 shadingNormal = normalize(v_Normal);
    if (u_isGrass)
        shadingNormal.y *= sign(shadingNormal.y) * u_lightDir.y; //intentionally left unnormalized

    float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 5, 0.0, 1.0));
    float diffuseComponent = max(dot(shadingNormal, u_lightDir), 0.0) * sunPositionAttenuation;

    vec3 Reflect = reflect(-u_lightDir, shadingNormal);
    vec3 ViewDir = normalize(u_viewPosition - v_FragPos);
    float specularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 4.0) * 0.75 * sunPositionAttenuation;

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
    ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

    if (u_shadowEnable)
    {
        float luminosity = calculateLuminosity();
        diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
        specularColor = luminosity * specularComponent * sampledSpecular.rgb;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(luminosity, diffuseComponent + SHADOW_INFLUENCE));
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
        o_FragColor += clamp(o_FragColor * shadingNormal.y * 0.5, -0.04, 0.0);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        specularColor = sampledSpecular.rgb * specularComponent;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        o_FragColor += clamp(o_FragColor * shadingNormal.y * 0.5, -0.04, 0.0);
    }

    if(u_useLandBlending)
        o_FragColor.a = mix(0.0, 1.0, v_AlphaValue);
}
