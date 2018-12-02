#version 450

out vec4 o_FragColor;

in vec3 v_FragPos;
in vec2 v_TexCoords;

uniform sampler2D u_land_diffuse[2];
uniform sampler2D u_diffuse_mix_map;
uniform sampler2D u_normal_map;
uniform vec3      u_lightDir;
uniform float     u_mapDimension;
uniform bool      u_shadowEnable;
uniform float     u_ambientDay;
uniform float     u_ambientNight;
uniform vec3      u_viewPosition;
uniform mat4      u_lightSpaceMatrix[2];
uniform float     u_shadowNearDistance;

const vec3  NORMAL = vec3(0.0, 1.0, 0.0);
const float MAX_DESATURATING_VALUE = 0.5;

@include shadowSampling.ifs
@include desaturationFunc.ifs

void main()
{
    float DiffuseTextureMix = texture(u_diffuse_mix_map, v_FragPos.xz * u_mapDimension + 0.5).r;
    vec4 sampledDiffuse = mix(texture(u_land_diffuse[0], v_TexCoords),
                              texture(u_land_diffuse[1], v_TexCoords),
                              DiffuseTextureMix);

    @include shadingVariables.ifs

    vec3 ShadingNormal = clamp((texture(u_normal_map, v_FragPos.xz * 0.125).xzy) * 2.2, vec3(0.0), vec3(1.0));
    ShadingNormal.xyz -= vec3(0.5);
    ShadingNormal = normalize(NORMAL + 1.5 * ShadingNormal);

    float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));
    float diffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0) * sunPositionAttenuation;

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
    ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

    if (u_shadowEnable)
    {
        vec4 fragPosLightSpaceNear = u_lightSpaceMatrix[0] * vec4(v_FragPos, 1.0);
        vec3 projectedCoordsNear = fragPosLightSpaceNear.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]
        int shadowMapIndex;
        vec3 projectedCoords;
        if (projectedCoordsNear.x > 0.0 && projectedCoordsNear.x < 1.0 &&
            projectedCoordsNear.y > 0.0 && projectedCoordsNear.y < 1.0 &&
            projectedCoordsNear.z > 0.1 && projectedCoordsNear.z < 1.0)
        {
            shadowMapIndex = 0;
            projectedCoords = projectedCoordsNear;
        }
        else
        {
            shadowMapIndex = 1;
            vec4 fragPosLightSpaceFar = u_lightSpaceMatrix[1] * vec4(v_FragPos, 1.0);
            vec3 projectedCoordsFar = fragPosLightSpaceFar.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]
            projectedCoords = projectedCoordsFar;
        }
        float luminosity = ext_calculateLuminosity(shadowMapIndex, projectedCoords);

        diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
        resultColor = ambientColor + diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE));
        ext_desaturate(o_FragColor, desaturatingValue);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        resultColor = ambientColor + diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }
}
