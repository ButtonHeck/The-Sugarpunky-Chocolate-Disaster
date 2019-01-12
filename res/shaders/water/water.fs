#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;

uniform sampler2D   u_specular_map;
uniform sampler2D   u_normal_map;
uniform sampler2D   u_dudv_map;
uniform sampler2D   u_reflectionMap;
uniform sampler2D   u_refractionMap;
uniform vec3        u_lightDir;
uniform vec3        u_viewPosition;
uniform bool        u_debugRenderMode;
uniform float       u_ambientDay;
uniform float       u_ambientNight;
uniform float       u_dudvMoveOffset;
uniform float       U_SCREEN_WIDTH;
uniform float       U_SCREEN_HEIGHT;

const float MAX_DESATURATING_VALUE = 0.5;
const vec3  KISSEL_COLOR = vec3(107.0, 30.0, 15.0) / 255.0;
const float KISSEL_ALPHA_MIN = 0.7;
const float REFLECTION_MIX_DAY = 0.3;
const float REFLECTION_MIX_NIGHT = 0.5;
const float DUDV_INFLUENCE = 0.008;
const float TEXTURE_TILING = 1.0 / 8.0;
const int   SPECULAR_SHININESS = 8;
const vec2  SCREEN_SPACE_DIVISOR = vec2(U_SCREEN_WIDTH, U_SCREEN_HEIGHT);

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

        vec2 fragPosTexCoords = v_FragPos.xz * TEXTURE_TILING;
        vec2 dudvTextureOffset = (texture(u_dudv_map, fragPosTexCoords + vec2(u_dudvMoveOffset)).rg * 2.0 - 1.0)
                                  * DUDV_INFLUENCE;
        vec2 screenSpaceTexCoordsReflection = gl_FragCoord.xy / SCREEN_SPACE_DIVISOR;
        vec2 screenSpaceTexCoordsRefraction = screenSpaceTexCoordsReflection;
        screenSpaceTexCoordsReflection.y = 1.0 - screenSpaceTexCoordsReflection.y;

        vec3 ShadingNormal = texture(u_normal_map, fragPosTexCoords + dudvTextureOffset).xzy;
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
        float specularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 32.0) * SPECULAR_SHININESS * fresnelEffect;
        vec3 sampledSpecular = texture(u_specular_map, fragPosTexCoords.yx + dudvTextureOffset).rgb * sunPositionAttenuation;

        //world reflection
        vec4 sampledWorldReflection = vec4(texture(u_reflectionMap, screenSpaceTexCoordsReflection + dudvTextureOffset + vec2(ShadingNormal.xz) * 0.125).rgb, 1.0);

        //world refraction
        vec4 sampledWorldRefraction = vec4(texture(u_refractionMap, screenSpaceTexCoordsRefraction + dudvTextureOffset + vec2(ShadingNormal.xz) * 0.125).rgb, 1.0);

        //shadowing
        int shadowMapIndex;
        vec3 projectedCoords;
        ext_calculateShadowMapIndexAndProjectedCoords(shadowMapIndex, projectedCoords);
        projectedCoords.xy += dudvTextureOffset;
        float luminosity = ext_calculateLuminosity3(shadowMapIndex, projectedCoords, u_bias);

        diffuseColor = luminosity * sampledDiffuse * diffuseComponent;
        specularColor = specularComponent * sampledSpecular;
        resultColor = ambientColor + diffuseColor + specularColor;

        o_FragColor = vec4(mix(resultColor + sampledWorldRefraction.rgb, sampledWorldReflection.rgb, fresnelEffect), fresnelEffect + KISSEL_ALPHA_MIN);

        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE));
        ext_desaturate(o_FragColor, desaturatingValue);
    }
}
