#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;

uniform samplerCube u_skybox;
uniform sampler2D   u_specular_map;
uniform sampler2D   u_normal_map;
uniform sampler2D   u_dudv_map;
uniform vec3        u_lightDir;
uniform vec3        u_viewPosition;
uniform bool        u_debugRenderMode;
uniform float       u_ambientDay;
uniform float       u_ambientNight;
uniform float       u_dudvMoveOffset;

const float MAX_DESATURATING_VALUE = 0.5;
const vec3  KISSEL_COLOR = vec3(107.0, 30.0, 15.0) / 255.0;
const float KISSEL_ALPHA_MIN = 0.7;
const float REFLECTION_MIX_DAY = 0.25;
const float REFLECTION_MIX_NIGHT = 0.025;
const float DUDV_INFLUENCE = 0.008;

@include shadowSampling.ifs
@include desaturationFunc.ifs

void main()
{
    if (u_debugRenderMode)
        o_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
    {
        vec3 sampledDiffuse = KISSEL_COLOR;

        @include shadingVariables.ifs

        vec2 texCoords = v_FragPos.xz * 0.125;
        vec2 dudvTextureOffset = (texture(u_dudv_map, texCoords + vec2(u_dudvMoveOffset)).rg * 2.0 - 1.0)
                                  * DUDV_INFLUENCE;

        vec3 ShadingNormal = texture(u_normal_map, texCoords + dudvTextureOffset).xzy;
        ShadingNormal.xz *= 2.0;
        ShadingNormal.xz -= 1.0;
        ShadingNormal = normalize(v_Normal + ShadingNormal);

        vec3 ViewDir = normalize(u_viewPosition - v_FragPos);
        float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));

        //fresnel
        float fresnelEffect = (1.0 - clamp(dot(ViewDir, ShadingNormal) + 0.2, 0.0, 1.0))
                               * mix(REFLECTION_MIX_NIGHT, REFLECTION_MIX_DAY, clamp(u_lightDir.y * 10, 0.0, 1.0));

        //diffuse
        float diffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0) * sunPositionAttenuation;

        ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
        ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

        //specular
        vec3 Reflect = reflect(-u_lightDir, ShadingNormal);
        float specularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 32.0) * 8 * fresnelEffect;
        vec3 sampledSpecular = texture(u_specular_map, texCoords.yx + dudvTextureOffset).rgb * sunPositionAttenuation;

        //reflect skybox component
        vec3 skyboxCoords = reflect(-ViewDir, ShadingNormal);
        skyboxCoords.xy += dudvTextureOffset;
        vec4 sampledDiffuseSkybox = vec4(texture(u_skybox, skyboxCoords).rgb, 1.0);

        //shadowing
        int shadowMapIndex;
        vec3 projectedCoords;
        ext_calculateShadowMapIndexAndProjectedCoords(shadowMapIndex, projectedCoords);
        float luminosity = ext_calculateLuminosity3(shadowMapIndex, projectedCoords, u_bias);

        diffuseColor = luminosity * sampledDiffuse * diffuseComponent;
        specularColor = specularComponent * sampledSpecular;
        resultColor = ambientColor + diffuseColor + specularColor;

        o_FragColor = vec4(mix(resultColor, sampledDiffuseSkybox.rgb, fresnelEffect), fresnelEffect + KISSEL_ALPHA_MIN);

        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE));
        ext_desaturate(o_FragColor, desaturatingValue);
    }
}
