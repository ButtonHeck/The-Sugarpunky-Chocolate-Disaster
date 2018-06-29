#version 450

out vec4 o_FragColor;

in vec2  v_TexCoords;
in float v_PosHeight;
in float v_TextureFlatMixRatio;
in float v_TextureHillMixRatio;
in float v_DiffuseComponentHill;
in float v_DiffuseComponentFlat;
in float v_SpecularComponent;
in vec3  v_Normal;
in vec3  v_ProjectedCoords;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_hills_diffuse;
uniform sampler2D u_hills_diffuse2;
uniform sampler2D u_hills_specular;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;

const float TEXEL_SIZE_MULTIPLIER_OFFSET = 1.0 + clamp((v_PosHeight * 10.0), 0.0, 1.5);
const vec2  TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float SHADOW_INFLUENCE = 0.3;
const float MAX_DESATURATING_VALUE = 0.8 / (1.0 - SHADOW_INFLUENCE);
const float DIFFUSE_MIX = 0.5;

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
    vec3 gray = vec3(colorMedian);
    vec4 desaturated = vec4(mix(fragColor.rgb, gray, desaturatingValue), fragColor.a);
    return desaturated;
}

void main()
{
    vec4 sampledDiffuse =
        mix(mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), v_TextureFlatMixRatio),
            mix(texture(u_hills_diffuse, v_TexCoords), texture(u_hills_diffuse2, v_TexCoords), v_TextureHillMixRatio),
            clamp(v_PosHeight, 0.0, 1.0));
    vec4 sampledSpecular =
        mix(vec4(0.0), texture(u_hills_specular, v_TexCoords), clamp(v_PosHeight, 0.0, 1.0));

    vec3 diffuseColor;
    vec3 specularColor;
    vec3 resultColor;
    float diffuseComponent = mix(v_DiffuseComponentFlat, v_DiffuseComponentHill, clamp(v_PosHeight, 0.0, 1.0));

    if (u_shadowEnable)
    {
        float luminosity = calculateLuminosity(v_Normal);
        diffuseColor = luminosity * mix(sampledDiffuse.rgb, sampledDiffuse.rgb * diffuseComponent, DIFFUSE_MIX);
        specularColor = v_SpecularComponent * sampledSpecular.rgb;
        resultColor = diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity - (1.0 - SHADOW_INFLUENCE));
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = mix(sampledDiffuse.rgb, sampledDiffuse.rgb * diffuseComponent, DIFFUSE_MIX);
        specularColor = v_SpecularComponent * sampledSpecular.rgb;
        resultColor = diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }
}
