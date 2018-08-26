#version 450

out vec4 o_FragColor;

in vec3  v_SkyboxCoords;
in float v_DiffuseComponent;
in float v_SpecularComponent;
in vec3  v_FragPos;

uniform samplerCube u_skybox;
uniform sampler2D   u_normal_map;
uniform float       u_mapDimension;
uniform float       U_WATER_LEVEL;

const vec3 KISSEL_COLOR = vec3(0.4, 0.1, 0.05);
const vec3 KISSEL_REFLECT_COLOR = vec3(0.9, 0.75, 0.75);
const float DIFFUSE_MIX = 0.2;
const float REFLECTION_MIX = 0.075;
const float KISSEL_ALPHA = 0.7;

void main()
{
    vec3 colorAttenuation = texture(u_normal_map, v_FragPos.xz * u_mapDimension + 0.5).rgb * 0.0625;
    vec4 sampledDiffuseSkybox = vec4(texture(u_skybox, v_SkyboxCoords).rgb, 1.0);
    float colorSpread = texture(u_normal_map, v_FragPos.xz * 0.25).g * 0.2 + 0.9;
    float waterFragmentPosY = v_FragPos.y + U_WATER_LEVEL;

    vec3 diffuseColor = mix(KISSEL_COLOR * (1.9 + waterFragmentPosY * 1.4) * colorSpread,
                            KISSEL_COLOR * v_DiffuseComponent,
                            DIFFUSE_MIX);
    vec3 specularColor = v_SpecularComponent * KISSEL_REFLECT_COLOR * (1.0 + waterFragmentPosY * 1.4) * (colorSpread + 0.5);
    vec3 resultColor = diffuseColor + specularColor - colorAttenuation;

    o_FragColor = vec4(mix(resultColor, sampledDiffuseSkybox.rgr, REFLECTION_MIX), KISSEL_ALPHA);
}
