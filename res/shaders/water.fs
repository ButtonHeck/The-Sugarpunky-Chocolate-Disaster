#version 450

out vec4 o_FragColor;

in vec3  vg_SkyboxCoords;
in float vg_DiffuseComponent;
in float vg_SpecularComponent;
in vec3  vg_FragPos;

uniform samplerCube u_skybox;
uniform sampler2D   u_normal_map;
uniform float       u_mapDimension;

const vec3 KISSEL_COLOR = vec3(0.4, 0.1, 0.05);
const float DIFFUSE_MIX = 0.2;
const float REFLECTION_MIX = 0.075;
const float KISSEL_ALPHA = 0.7;

void main()
{
    vec3 colorAttenuation = texture(u_normal_map, vg_FragPos.xz * u_mapDimension + 0.5).rgb * 0.0625;
    vec4 sampledDiffuseSkybox = vec4(texture(u_skybox, vg_SkyboxCoords).rgb, 1.0);

    vec3 diffuseColor = mix(KISSEL_COLOR, KISSEL_COLOR * vg_DiffuseComponent, DIFFUSE_MIX);
    vec3 specularColor = vg_SpecularComponent * KISSEL_COLOR;
    vec3 resultColor = diffuseColor + specularColor - colorAttenuation;

    o_FragColor = vec4(mix(resultColor, sampledDiffuseSkybox.rgr, REFLECTION_MIX), KISSEL_ALPHA);
}
