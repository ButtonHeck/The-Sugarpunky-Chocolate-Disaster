#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;
in vec3  v_ViewDir;

uniform samplerCube u_skybox;
uniform sampler2D   u_diffuse_mix_map;
uniform sampler2D   u_specular_map;
uniform sampler2D   u_normal_map;
uniform vec3        u_lightDir;
uniform float       u_mapDimension;
uniform float       U_WATER_LEVEL;

const vec3 KISSEL_COLOR = vec3(0.37, 0.07, 0.05);
const vec3 KISSEL_REFLECT_COLOR = vec3(0.9, 0.75, 0.75);
const float REFLECTION_MIX = 0.1;
const float KISSEL_ALPHA = 0.7;

void main()
{
    vec3 colorAttenuation = texture(u_diffuse_mix_map, v_FragPos.xz * u_mapDimension + 0.5).rgb * 0.05;
    float waterFragmentPosY = v_FragPos.y + U_WATER_LEVEL;

    //swizzle z and y to rotate Z-aligned normal map 90 degrees around X axis, as like we look at it upside down
    //also scale up texture mapping a bit
    vec3 ShadingNormal = normalize(texture(u_normal_map, v_FragPos.xz * 0.0625 * 0.5).xzy);
    float diffuseComponent = dot(ShadingNormal, u_lightDir);

    //specular
    vec3 ReflectNormal = normalize(ShadingNormal + v_Normal);
    vec3 Reflect = reflect(-u_lightDir, ReflectNormal);
    float specularComponent = pow(max(dot(Reflect, v_ViewDir), 0.0), 8.0);
    vec3 sampledSpecular = texture(u_specular_map, v_FragPos.zx * 0.0625).rgb;

    //reflect skybox component
    vec3 skyboxCoords = reflect(-v_ViewDir, ReflectNormal);
    vec4 sampledDiffuseSkybox = vec4(texture(u_skybox, skyboxCoords).rgb, 1.0);

    vec3 diffuseColor = KISSEL_COLOR * diffuseComponent;
    vec3 specularColor = specularComponent * sampledSpecular * (1.0 + waterFragmentPosY);
    vec3 resultColor = diffuseColor + specularColor - colorAttenuation;

    o_FragColor = vec4(mix(resultColor, sampledDiffuseSkybox.rgr, REFLECTION_MIX), KISSEL_ALPHA);
}
