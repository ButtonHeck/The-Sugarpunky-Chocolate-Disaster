#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec2  v_TexCoords;
in float v_PosHeight;
in float v_TextureMixRatio;
in float v_DiffuseComponent;
in vec4  v_FragPosLightSpace;
in vec3  v_Normal;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_sand_diffuse;
uniform sampler2D u_sand_diffuse2;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;

const float POISSON_SHADOW_VALUE_GAIN = clamp(-v_PosHeight * 5.0, 0.0, 4.0);
const vec2 POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float calculateShadowComponent(vec4 fragPosLightSpace, vec3 normal)
{
    //transform from [-1;1] to [0;1]
    vec3 projCoords = fragPosLightSpace.xyz * 0.5 + 0.5;
    float closestDepth = texture(u_shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    float bias = max(0.0008 * (1.0 - dot(normal, u_lightDir)), 0.0005);
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);

    //poisson filtering
    for (int i = 0; i < 4; i++)
    {
        float poissonDiskDepth = texture(u_shadowMap, projCoords.xy + POISSON_DISK[i] * texelSize).r;
        shadow += currentDepth - bias / 2.0 > poissonDiskDepth ? 1.0 : 0.0;
    }
    shadow /= (4.0 + POISSON_SHADOW_VALUE_GAIN);

    if (projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    vec4 sampledDiffuse =
                mix(mix(texture(u_sand_diffuse, v_TexCoords), texture(u_sand_diffuse2, v_TexCoords), v_TextureMixRatio),
                    mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), v_TextureMixRatio),
                    max(min(v_PosHeight + 1.0, 1.0), 0.0));

    vec3 diffuseColor;
    if (u_shadowEnable)
    {
        float shadowInfluence = calculateShadowComponent(v_FragPosLightSpace, v_Normal);
        diffuseColor = sampledDiffuse.rgb * ((1.0 - shadowInfluence) * v_DiffuseComponent * 0.5 + 0.5) * (1 + v_FragPos.y / 2.5);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * (v_DiffuseComponent * 0.5 + 0.5) * (1 + v_FragPos.y / 2.5);
    }

    vec3 resultColor = diffuseColor;
    o_FragColor = vec4(resultColor, sampledDiffuse.a);
}
