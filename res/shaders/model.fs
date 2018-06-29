#version 450

out vec4 o_FragColor;

in vec2  v_TexCoords;
in vec3  v_Normal;
in float v_DiffuseComponent;
in float v_SpecularComponent;
in vec3  v_ProjectedCoords;

uniform sampler2D u_texture_diffuse;
uniform sampler2D u_texture_specular;
uniform bool      u_shadow;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;

const vec2  TEXEL_SIZE = 1.0 / textureSize(u_shadowMap, 0);
const float SHADOW_INFLUENCE = 0.3;
const float DIFFUSE_MIX = 0.6;
const float MAX_DESATURATING_VALUE = 0.8 / (1.0 - SHADOW_INFLUENCE);
const vec2  POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float calculateLuminosity(vec3 normal)
{
    if (dot(normal, u_lightDir) < 0)
        return (1.0 + dot(normal.xz, u_lightDir.xz) * (SHADOW_INFLUENCE + 0.1));

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
    vec3 gray = vec3(colorMedian);
    vec4 desaturated = vec4(mix(fragColor.rgb, gray, desaturatingValue), fragColor.a);
    return desaturated;
}

void main()
{
    vec4 sampledDiffuse = texture(u_texture_diffuse, v_TexCoords);
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
        float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, luminosity - (1.0 - SHADOW_INFLUENCE));
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
