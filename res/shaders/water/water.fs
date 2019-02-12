#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;

uniform sampler2D   u_specular_map;
uniform sampler2D   u_normalMap;
uniform sampler2D   u_dudv_map;
uniform sampler2D   u_reflectionMap;
uniform sampler2D   u_refractionMap;
uniform sampler2D   u_refractionDepthMap;
uniform vec3        u_lightDir;
uniform vec3        u_viewPosition;
uniform bool        u_debugRenderMode;
uniform float       u_ambientDay;
uniform float       u_ambientNight;
uniform float       u_dudvMoveOffset;
uniform float       U_SCREEN_WIDTH;
uniform float       U_SCREEN_HEIGHT;
uniform float       u_near;
uniform float       u_far;

const float MAX_DESATURATING_VALUE = 0.5;
const vec3  KISSEL_COLOR = vec3(107.0, 15.0, 5.0) / 255.0;
const float KISSEL_ALPHA_MIN = 0.9;
const float REFLECTION_MIX_DAY = 0.2;
const float REFLECTION_MIX_NIGHT = 0.5;
const float DUDV_INFLUENCE = 0.008;
const float TEXTURE_TILING = 1.0 / 8.0;
const vec2  SCREEN_SPACE_DIVISOR = vec2(U_SCREEN_WIDTH, U_SCREEN_HEIGHT);

@include shadowSampling.ifs
@include desaturationFunc.ifs

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * u_near) / (u_far + u_near - z * (u_far - u_near));
}

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

        vec3 ShadingNormal = texture(u_normalMap, fragPosTexCoords + dudvTextureOffset).xzy;
        ShadingNormal.xz *= 2.0;
        ShadingNormal.xz -= 1.0;
        ShadingNormal = normalize(v_Normal + ShadingNormal);

        vec3 ViewDir = normalize(u_viewPosition - v_FragPos);
        float sunPositionAttenuation = clamp((u_lightDir.y - 0.02) * 10, 0.0, 1.0);

        //fresnel
        float refractionInfluence = max(dot(ViewDir, ShadingNormal), 0.0);
        float reflectionInfluenceAlpha = (1.0 - refractionInfluence);
        float reflectionInfluenceColor = reflectionInfluenceAlpha * mix(REFLECTION_MIX_NIGHT, REFLECTION_MIX_DAY, sunPositionAttenuation);

        //diffuse
        float diffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0) * sunPositionAttenuation;

        ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
        ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

        //specular
        vec3 Reflect = reflect(-u_lightDir, ShadingNormal);
        float specularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 32.0);
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
        float specularLuminosityInfluence = 1.0 - ((1.0 - luminosity) * SHADOW_INFLUENCE_RECIPROCAL);
        specularColor = specularLuminosityInfluence * specularComponent * sampledSpecular * sunPositionAttenuation;
        resultColor = ambientColor + diffuseColor + specularColor;

        float refractionDepth = linearizeDepth(texture(u_refractionDepthMap, screenSpaceTexCoordsRefraction).r);
        float currentFragDepth = linearizeDepth(gl_FragCoord.z);
        float kisselDepth = min((refractionDepth - currentFragDepth) * 96, 1.0);

        o_FragColor = vec4(resultColor + mix(sampledWorldRefraction.rgb, sampledWorldReflection.rgb, reflectionInfluenceColor) * 0.5,
                           mix(kisselDepth, max(reflectionInfluenceAlpha, KISSEL_ALPHA_MIN), kisselDepth));

        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE));
        ext_desaturate(o_FragColor, desaturatingValue);
    }
}
