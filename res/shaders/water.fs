#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;

uniform samplerCube u_skybox;
uniform sampler2D   u_bottomRelief_diffuse;
uniform sampler2D   u_specular_map;
uniform sampler2D   u_normal_map;
uniform vec3        u_lightDir;
uniform float       u_mapDimension;
uniform vec3        u_viewPosition;

const vec3 KISSEL_COLOR = vec3(107.0, 30.0, 7.0) / 255.0;
const float REFLECTION_MIX = 0.075;
const float KISSEL_ALPHA = 0.5;

void main()
{
    //swizzle z and y to rotate Z-aligned normal map 90 degrees around X axis, as like we look at it upside down
    //also scale up texture mapping a bit
    vec3 sampledNormal = normalize(texture(u_normal_map, v_FragPos.zx * 0.25 * 0.5).xzy);
    vec3 ShadingNormal = normalize(v_Normal + sampledNormal);
    vec3 specularNormal = ShadingNormal;
    specularNormal.y *= 0.75;
    specularNormal = normalize(specularNormal);
    float diffuseComponent = dot(ShadingNormal, u_lightDir);

    //specular
    vec3 Reflect = reflect(-u_lightDir, specularNormal);
    vec3 ViewDir = normalize(u_viewPosition - v_FragPos);
    float specularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 16.0);
    vec3 sampledSpecular = texture(u_specular_map, v_FragPos.zx * 0.125).rgb;

    //reflect skybox component
    vec3 skyboxCoords = reflect(-ViewDir, ShadingNormal);
    vec4 sampledDiffuseSkybox = vec4(texture(u_skybox, skyboxCoords).rgb, 1.0);

    vec3 diffuseColor = KISSEL_COLOR * diffuseComponent;
    vec3 specularColor = specularComponent * sampledSpecular;
    vec3 resultColor = diffuseColor + specularColor;

    float alphaAttenuation = texture(u_bottomRelief_diffuse, v_FragPos.xz * u_mapDimension + 0.5).r * 0.5;
    o_FragColor = vec4(mix(resultColor, sampledDiffuseSkybox.rgb, REFLECTION_MIX), KISSEL_ALPHA + alphaAttenuation);
}
