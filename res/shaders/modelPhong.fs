#version 450
layout (early_fragment_tests) in;

out vec4 o_FragColor;

in vec2  v_TexCoords;
in vec3  v_Normal;
in vec3  v_ProjectedCoords;
in float v_FlatBlend;
in vec3  v_FragPos;

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_specular;
uniform vec3      u_viewPosition;
uniform bool      u_shadow;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
uniform bool      u_useFlatBlending;

const vec2  TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float SHADOW_INFLUENCE = 0.3;
const float SHADOW_INFLUENCE_FOR_NEGATIVE_DOT = 0.4;
const float ONE_MINUS_SHADOW_INFLUENCE = 1.0 - SHADOW_INFLUENCE;
const float MAX_DESATURATING_VALUE = 0.8 / ONE_MINUS_SHADOW_INFLUENCE;
const vec2  POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

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

void main()
{
    vec4 sampledDiffuse = texture(u_texture_diffuse1, v_TexCoords);
    vec4 sampledSpecular = texture(u_texture_specular, v_TexCoords);
    vec3 ambientColor = 0.25 * sampledDiffuse.rgb;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 resultColor;

    float luminosity = 0.0;
    vec3 shadingNormal = normalize(v_Normal);
    float diffuseComponent = max(dot(shadingNormal, u_lightDir), 0.0);
    vec3 Reflect = reflect(-u_lightDir, shadingNormal);
    vec3 ViewDir = normalize(u_viewPosition - v_FragPos);
    float specularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 8.0) * 0.5;

    if (u_shadowEnable)
    {
        luminosity = calculateLuminosity(shadingNormal);
        diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
        specularColor = luminosity * specularComponent * sampledSpecular.rgb;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity - ONE_MINUS_SHADOW_INFLUENCE);
        o_FragColor = desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        specularColor = sampledSpecular.rgb * specularComponent;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }

    if (u_shadow)
        o_FragColor += clamp(o_FragColor * shadingNormal.y * 0.5, -0.05, 0.01);

    //perform flat blending
    if(u_useFlatBlending)
    {
        float flatBlend = clamp(v_FlatBlend, 0.0, 1.0);
        o_FragColor.a = mix(0.0, 1.0, flatBlend);
    }
}
