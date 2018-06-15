#version 450

out vec4 o_FragColor;

in vec3 v_FragPos;
in vec2 v_TexCoords;
in vec3 v_LightDir;
in vec4 v_FragPosLightSpace;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_normal_map;
uniform int       u_mapDimension;
uniform sampler2D u_shadowMap;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);

float calculateShadowComponent(vec4 fragPosLightSpace, vec3 normal)
{
    //perform perspective divide, unnecessary for orthographic projection
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //transform from [-1;1] to [0;1]
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = 0.0f;
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if (projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    vec3 FlatNormal = texture(u_normal_map, vec2(v_FragPos.x / u_mapDimension + 0.5, v_FragPos.z / u_mapDimension + 0.5)).rgb;
    vec4 sampledDiffuse = mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), FlatNormal.r);
    vec3 ShadingNormal = normalize(NORMAL + FlatNormal);
    float shadowInfluence = calculateShadowComponent(v_FragPosLightSpace, NORMAL);

    //diffuse
    float diffuseComponent = max(dot(ShadingNormal, v_LightDir), 0.0);
    vec3 diffuseColor = sampledDiffuse.rgb * ((1.0 - shadowInfluence) * diffuseComponent * 0.5 + 0.5);

    vec3 resultColor = diffuseColor;
    o_FragColor = vec4(resultColor, sampledDiffuse.a);
}
