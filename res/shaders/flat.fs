#version 450

out vec4 o_FragColor;

in vec3 v_FragPos;
in vec2 v_TexCoords;
in vec3 v_LightDir;
in vec4 v_FragPosLightSpace;
in vec2 v_TexelSize;
in vec3 v_ProjectedCoords;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_normal_map;
uniform float     u_mapDimension;
uniform sampler2D u_shadowMap;
uniform bool      u_shadowEnable;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);
const float SHADOW_INFLUENCE = 0.3;
const float SHADOW_BIAS = 0.00025;
const vec2 POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float calculateShadowComponent(vec4 fragPosLightSpace, vec3 normal)
{
    float closestDepth = texture(u_shadowMap, v_ProjectedCoords.xy).r;
    float currentDepth = v_ProjectedCoords.z;
    float shadow = 0.0f;

    //poisson filtering
    for (int i = 0; i < 4; i++)
    {
        float poissonDiskDepth = texture(u_shadowMap, v_ProjectedCoords.xy + POISSON_DISK[i] * v_TexelSize).r;
        shadow += currentDepth - SHADOW_BIAS > poissonDiskDepth ? 0.25 : 0.0;
    }
    return (1.0 - shadow * SHADOW_INFLUENCE);
}

void main()
{
    vec3 FlatNormal = texture(u_normal_map, v_FragPos.xz * u_mapDimension + 0.5).rgb;
    vec4 sampledDiffuse = mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), FlatNormal.r);
    vec3 ShadingNormal = normalize(NORMAL + FlatNormal);

    vec3 diffuseColor;
    float diffuseComponent = max(dot(ShadingNormal, v_LightDir), 0.0);
    if (u_shadowEnable)
    {
        float shadowComponent = calculateShadowComponent(v_FragPosLightSpace, NORMAL);
        diffuseColor = shadowComponent * sampledDiffuse.rgb * (diffuseComponent * 0.5 + 0.5);
    }
    else
        diffuseColor = sampledDiffuse.rgb * (diffuseComponent * 0.5 + 0.5);

    vec3 resultColor = diffuseColor;
    o_FragColor = vec4(resultColor, sampledDiffuse.a);
}
