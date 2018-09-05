#version 450
layout (early_fragment_tests) in;

out vec4 o_FragColor;

in vec2  v_TexCoords;
in vec3  v_Normal;
in float v_DiffuseComponent;
in float v_SpecularComponent;
in vec3  v_ProjectedCoords;

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_specular;
uniform bool      u_shadow;
uniform sampler2D u_shadowMap;
uniform sampler2D u_occlusionMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
uniform bool      u_occlusion;

uniform float     U_NEAR;
uniform float     U_FAR;
uniform float     U_SCR_WIDTH;
uniform float     U_SCR_HEIGHT;

const vec2  TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float SHADOW_INFLUENCE = 0.3;
const float SHADOW_INFLUENCE_FOR_NEGATIVE_DOT = 0.4;
const float ONE_MINUS_SHADOW_INFLUENCE = 1.0 - SHADOW_INFLUENCE;
const float DIFFUSE_MIX = 0.7;
const float MAX_DESATURATING_VALUE = 0.8 / ONE_MINUS_SHADOW_INFLUENCE;
const vec2  POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);
const float DEPTH_BIAS = 0.003;

float calculateLuminosity(vec3 normal)
{
    if (dot(normal, u_lightDir) < 0)
        return (1.0 + dot(normal.xz, u_lightDir.xz) * SHADOW_INFLUENCE_FOR_NEGATIVE_DOT);

    float closestDepth = texture(u_shadowMap, v_ProjectedCoords.xy).r;
    float currentDepth = v_ProjectedCoords.z;
    float shadow = 0.0f;
    float bias = max(0.0007 * (1.0 - dot(normal, u_lightDir)), 0.00007);

    //PCF filtering
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(u_shadowMap, v_ProjectedCoords.xy + vec2(x,y) * TEXEL_SIZE).r;
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

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * U_NEAR * U_FAR) / (U_FAR + U_NEAR - z * (U_FAR - U_NEAR));
}

void main()
{
    if (u_occlusion)
    {
        float ocMapDepth = linearizeDepth(texture(u_occlusionMap, vec2(gl_FragCoord.x / U_SCR_WIDTH, gl_FragCoord.y / U_SCR_HEIGHT)).r) / U_FAR;
        float fragDepth = linearizeDepth(gl_FragCoord.z) / U_FAR;
        if (fragDepth >= ocMapDepth + DEPTH_BIAS)
            discard;
    }

    vec4 sampledDiffuse = texture(u_texture_diffuse1, v_TexCoords);
    vec4 sampledSpecular = texture(u_texture_specular, v_TexCoords);
    vec3 ambientColor = 0.2 * sampledDiffuse.rgb;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 resultColor;

    if (u_shadowEnable)
    {
        float luminosity = calculateLuminosity(v_Normal);
        diffuseColor = luminosity * mix(sampledDiffuse.rgb, sampledDiffuse.rgb * v_DiffuseComponent, DIFFUSE_MIX);
        specularColor = v_SpecularComponent * sampledSpecular.rgb;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity - ONE_MINUS_SHADOW_INFLUENCE);
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = mix(sampledDiffuse.rgb, sampledDiffuse.rgb * v_DiffuseComponent, DIFFUSE_MIX);
        specularColor = v_SpecularComponent * sampledSpecular.rgb;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }

    if (u_shadow)
        o_FragColor += clamp(o_FragColor * v_Normal.y * 0.5, -0.05, 0.01);
}
