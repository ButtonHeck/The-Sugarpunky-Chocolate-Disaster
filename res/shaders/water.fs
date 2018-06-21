#version 450

out vec4 o_FragColor;

in vec3  vg_SkyboxCoords;
in float vg_DiffuseComponent;
in float vg_SpecularComponent;
in vec3  vg_FragPos;

uniform samplerCube u_skybox;
uniform sampler2D   u_normal_map;
uniform int         u_mapDimension;

const vec3 KISSEL_COLOR = vec3(0.4, 0.1, 0.05);

void main()
{
    vec3 colorAttenuation = texture(u_normal_map, vec2(vg_FragPos.x / u_mapDimension + 0.5, vg_FragPos.z / u_mapDimension + 0.5)).rgb / 16.0;
    vec4 sampledDiffuseSkybox = vec4(texture(u_skybox, vg_SkyboxCoords).rgb, 1.0);

    vec3 diffuseColor = KISSEL_COLOR * (vg_DiffuseComponent * 0.2 + 0.8);
    vec3 specularColor = vg_SpecularComponent * KISSEL_COLOR;
    vec3 resultColor = diffuseColor + specularColor - colorAttenuation;

    o_FragColor = vec4(0.925 * resultColor + 0.075 * sampledDiffuseSkybox.rgr, 0.7);
}
