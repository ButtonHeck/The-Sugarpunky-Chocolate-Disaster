#version 450

out vec4 o_FragColor;

in vec2  vg_TexCoords;
in vec3  vg_SkyboxCoords;
in float vg_DiffuseComponent;
in float vg_SpecularComponent;

uniform sampler2D   u_water_diffuse;
uniform sampler2D   u_water_specular;
uniform samplerCube u_skybox;

void main()
{
    vec4 sampledDiffuseSkybox = vec4(texture(u_skybox, vg_SkyboxCoords).rgb, 1.0);
    vec4 sampledDiffuse = texture(u_water_diffuse, vg_TexCoords);
    vec4 sampledSpecular = texture(u_water_specular, vg_TexCoords);

    vec3 diffuseColor = sampledDiffuse.rgb * (vg_DiffuseComponent * 0.4 + 0.6);
    vec3 specularColor = vg_SpecularComponent * sampledSpecular.rgb;
    vec3 resultColor = diffuseColor + specularColor;

    o_FragColor = vec4(0.5 * (resultColor + sampledDiffuseSkybox.rgb), sampledDiffuse.a);
}
