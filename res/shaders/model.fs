#version 450

out vec4 o_FragColor;

in vec2  v_TexCoords;
in vec3  v_Normal;
in float v_DiffuseComponent;
in float v_SpecularComponent;
in vec4  v_FragPosLightSpace;

uniform sampler2D u_texture_diffuse;
uniform sampler2D u_texture_specular;
uniform bool      u_shadow;
uniform sampler2D u_shadowMap;
uniform vec3      u_lightDir;

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
    float shadow = 0.0f;
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
    float bias = max(0.0007 * (1.0 - dot(normal, u_lightDir)), 0.00007);

    //poisson filtering
    for (int i = 0; i < 4; i++)
    {
        float poissonDiskDepth = texture(u_shadowMap, projCoords.xy + POISSON_DISK[i] * texelSize).r;
        shadow += currentDepth - bias > poissonDiskDepth ? 1.0 : 0.0;
    }
    shadow /= 3.0;

    if (projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    vec4 sampledDiffuse = texture(u_texture_diffuse, v_TexCoords);
    vec4 sampledSpecular = texture(u_texture_specular, v_TexCoords);
    float shadowInfluence = calculateShadowComponent(v_FragPosLightSpace, v_Normal);
    vec3 ambientColor = 0.2 * sampledDiffuse.rgb;
    vec3 diffuseColor = sampledDiffuse.rgb * ((1.0 - shadowInfluence) * v_DiffuseComponent * 0.6 + 0.4);
    vec3 specularColor = v_SpecularComponent * sampledSpecular.rgb;
    vec3 resultColor = ambientColor + diffuseColor + specularColor;

    o_FragColor = vec4(resultColor, sampledDiffuse.a);
    if (u_shadow)
        o_FragColor += clamp(o_FragColor * v_Normal.y / 2, -0.05, 0.01);
}
