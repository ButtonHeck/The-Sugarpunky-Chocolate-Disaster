#version 450

out vec4 o_FragColor;

in vec2  v_FragPosXZ;
in vec2  v_TexCoords;

uniform sampler2D u_underwater_diffuse;
uniform sampler2D u_bottomRelief_diffuse;
uniform sampler2D u_normalMap;
uniform float     u_mapDimensionReciprocal;
uniform vec3      u_lightDir;
uniform float     u_ambientDay;
uniform float     u_ambientNight;

const vec3  NORMAL = vec3(0.0, 1.0, 0.0);
const float MAX_DESATURATING_VALUE = 0.3;

@include desaturationFunc.ifs

void main()
{
    vec4 sampledDiffuse = texture(u_underwater_diffuse, v_TexCoords);

    @include shadingVariables.ifs

    vec3 ShadingNormal = texture(u_normalMap, v_FragPosXZ * 0.125).xzy;
    ShadingNormal.xyz -= vec3(0.5);
    ShadingNormal = normalize(NORMAL + ShadingNormal);

    float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));
    float diffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0) * sunPositionAttenuation;

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
    ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

    float reliefAttenuation = 1.0 - texture(u_bottomRelief_diffuse, v_FragPosXZ * u_mapDimensionReciprocal + 0.5).r;
    diffuseColor = sampledDiffuse.rgb * diffuseComponent * reliefAttenuation;
    resultColor = ambientColor + diffuseColor;
    o_FragColor = vec4(resultColor, sampledDiffuse.a);
    float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, diffuseComponent);
    ext_desaturate(o_FragColor, desaturatingValue);
}
