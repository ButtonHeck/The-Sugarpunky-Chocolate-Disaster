#version 450

out vec4 o_FragColor;

in vec2  v_FragPosXZ;
in vec2  v_TexCoords;

uniform sampler2D u_underwater_diffuse;
uniform sampler2D u_bottomRelief_diffuse;
uniform sampler2D u_normal_map;
uniform float     u_mapDimension;
uniform vec3      u_lightDir;

const vec3  NORMAL = vec3(0.0, 1.0, 0.0);
const float MAX_DESATURATING_VALUE = 0.7;

vec4 desaturate(vec4 fragColor, float desaturatingValue)
{
    float colorMedian = (fragColor.r + fragColor.g + fragColor.b) * 0.333;
    vec4 desaturated = vec4(mix(fragColor.rgb, vec3(colorMedian), desaturatingValue), fragColor.a);
    return desaturated;
}

void main()
{
    vec4 sampledDiffuse = texture(u_underwater_diffuse, v_TexCoords);

    vec3 ambientColorDaySelf = 0.08 * sampledDiffuse.rgb;
    vec3 ambientColorNightSelf = 0.03 * sampledDiffuse.rgb;
    vec3 nightAmbientColor = vec3(0.0034, 0.0012, 0.0009);
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 resultColor;

    vec3 ShadingNormal = clamp((texture(u_normal_map, v_FragPosXZ * 0.125).xzy) * 2.2, vec3(0.0), vec3(1.0));
    ShadingNormal.xyz -= vec3(0.5);
    ShadingNormal = normalize(NORMAL + ShadingNormal);

    float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));
    float diffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0) * sunPositionAttenuation;

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
    ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

    float reliefAttenuation = 1.0 - texture(u_bottomRelief_diffuse, v_FragPosXZ * u_mapDimension + 0.5).r;
    diffuseColor = sampledDiffuse.rgb * diffuseComponent * reliefAttenuation;
    resultColor = ambientColor + diffuseColor;
    o_FragColor = vec4(resultColor, sampledDiffuse.a);
    float desaturatingValue = mix(0.0, MAX_DESATURATING_VALUE, diffuseComponent);
    o_FragColor = desaturate(o_FragColor, desaturatingValue);
}
