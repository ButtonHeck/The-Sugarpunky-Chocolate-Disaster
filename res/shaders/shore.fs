#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec2  v_TexCoords;
in float v_PosHeight;
in float v_TextureMixRatio;
in float v_DiffuseComponent;
in vec3  v_Normal;
in vec3  v_ProjectedCoords;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_sand_diffuse;
uniform sampler2D u_sand_diffuse2;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;

const float DIFFUSE_MIX = 0.5;
const float SHADOW_INFLUENCE = 0.3;
const float MAX_DESATURATING_VALUE = 0.8 / (1.0 - SHADOW_INFLUENCE);
const vec2 TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float POISSON_SHADOW_VALUE_GAIN = clamp(-v_PosHeight * 5.0, 0.0, 4.0);
const vec2 POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float calculateLuminosity(vec3 normal)
{
    float closestDepth = texture(u_shadowMap, v_ProjectedCoords.xy).r;
    float currentDepth = v_ProjectedCoords.z;
    float shadow = 0.0;
    float bias = max(0.0004 * (1.0 - dot(normal, u_lightDir)), 0.00025);

    //poisson filtering
    for (int i = 0; i < 4; i++)
    {
        float poissonDiskDepth = texture(u_shadowMap, v_ProjectedCoords.xy + POISSON_DISK[i] * TEXEL_SIZE).r;
        shadow += currentDepth - bias > poissonDiskDepth ? 1.0 : 0.0;
    }
    shadow /= (4.0 + POISSON_SHADOW_VALUE_GAIN);
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
                mix(mix(texture(u_sand_diffuse, v_TexCoords), texture(u_sand_diffuse2, v_TexCoords), v_TextureMixRatio),
                    mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), v_TextureMixRatio),
                    clamp(v_PosHeight + 1.0, 0.0, 1.0));

    vec3 diffuseColor;
    vec3 resultColor;

    if (u_shadowEnable)
    {
        float luminosity = calculateLuminosity(v_Normal);
        diffuseColor = luminosity * mix(sampledDiffuse.rgb, sampledDiffuse.rgb * v_DiffuseComponent, DIFFUSE_MIX) * (1 + v_FragPos.y * 0.4);
        resultColor = diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity - (1.0 - SHADOW_INFLUENCE));
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = mix(sampledDiffuse.rgb, sampledDiffuse.rgb * v_DiffuseComponent, DIFFUSE_MIX) * (1 + v_FragPos.y * 0.4);
        resultColor = diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }
}
