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
const float SHADOW_INFLUENCE = 0.5;
const float DIFFUSE_MIX = 0.6;
const vec2  POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float calculateShadowComponent(vec3 normal)
{
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

void main()
{
    vec4 sampledDiffuse = texture(u_texture_diffuse, v_TexCoords);
    vec4 sampledSpecular = texture(u_texture_specular, v_TexCoords);
    vec3 ambientColor = 0.2 * sampledDiffuse.rgb;
    vec3 diffuseColor;

    if (u_shadowEnable)
    {
        float shadowInfluence = calculateShadowComponent(v_Normal);
        diffuseColor = shadowInfluence * mix(sampledDiffuse.rgb, sampledDiffuse.rgb * v_DiffuseComponent, DIFFUSE_MIX);
    }
    else
        diffuseColor = mix(sampledDiffuse.rgb, sampledDiffuse.rgb * v_DiffuseComponent, DIFFUSE_MIX);

    vec3 specularColor = v_SpecularComponent * sampledSpecular.rgb;
    vec3 resultColor = ambientColor + diffuseColor + specularColor;

    o_FragColor = vec4(resultColor, sampledDiffuse.a);
    if (u_shadow)
        o_FragColor += clamp(o_FragColor * v_Normal.y * 0.5, -0.05, 0.01);
}
