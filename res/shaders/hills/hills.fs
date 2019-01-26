#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec3  v_Normal;
in mat3  v_TNB;
in vec2  v_TexCoords;
in float v_TerrainTypeMix;
in float v_SpecularComponent;

uniform sampler2D u_land_diffuse[2];
uniform sampler2D u_hills_diffuse[2];
uniform sampler2D u_hills_specular;
uniform sampler2D u_diffuse_mix_map;
uniform float     u_mapDimension;
uniform float     u_maxHillHeight;
uniform sampler2D u_normal_map;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
uniform bool      u_debugRenderMode;
uniform float     u_ambientDay;
uniform float     u_ambientNight;
uniform vec3      u_viewPosition;

const vec3  NORMAL = vec3(0.0, 1.0, 0.0);
const float MAX_DESATURATING_VALUE_HILL = 0.5;
const float MAX_DESATURATING_VALUE_LAND = 0.5;
const float MIN_CANYON_CIRCLE_HEIGHT = 2.5;
const int   NUM_CANYON_CIRCLES = 12;

@include shadowSampling.ifs
@include desaturationFunc.ifs

void createCanyonRings(inout vec4 fragColor,
                       vec3 circleColor,
                       float minHeight,
                       float maxHeight,
                       int numCircles,
                       int circlesRandomMultiplier,
                       float normalY)
{
    vec2 canyonDistrubution = texture(u_diffuse_mix_map, v_FragPos.xz * u_mapDimension + 0.5).rg;
    float[2] canyonCircleOffset = {canyonDistrubution.r, canyonDistrubution.g};
    float canyonCircleStep = (maxHeight - minHeight) / (numCircles);
    vec3 canyonColor = circleColor * (1.0 - normalY);
    float influenceStep = 0.1 / (numCircles - 1);
    for (int i = 0; i < numCircles; i++)
    {
        float influence = 1.0 + i * influenceStep;
        float density = 2.0 + influence;
        fragColor.xyz += (influence - clamp(abs(density * (v_FragPos.y
                                                - (minHeight + i * canyonCircleStep))
                                                + canyonCircleOffset[i%2] * circlesRandomMultiplier),
                                            0.0, influence))
                         * canyonColor;
    }
}

void main()
{
    if (u_debugRenderMode)
        o_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
    {
        float DiffuseTextureMix = texture(u_diffuse_mix_map, v_FragPos.xz * u_mapDimension + 0.5).r;
        float TerrainTypeMixClamped = clamp(v_TerrainTypeMix, 0.0, 1.0);
        vec4 sampledDiffuse = mix(mix(texture(u_land_diffuse[0], v_TexCoords * 2.0),
                                      texture(u_land_diffuse[1], v_TexCoords * 2.0),
                                      DiffuseTextureMix),
                                  mix(texture(u_hills_diffuse[0], v_TexCoords),
                                      texture(u_hills_diffuse[1], v_TexCoords),
                                      DiffuseTextureMix),
                                  TerrainTypeMixClamped);
        vec4 sampledSpecular = mix(vec4(0.0), texture(u_hills_specular, v_TexCoords), TerrainTypeMixClamped);

        @include shadingVariables.ifs

        //we need this to be swizzled for color-smooth land blending
        vec3 ShadingNormal = texture(u_normal_map, v_FragPos.xz * 0.125).xzy;
        ShadingNormal -= vec3(0.5);
        vec3 ShadingNormalLand = normalize(NORMAL + 5.0 * ShadingNormal);
        vec3 ShadingNormalHill = normalize(v_Normal + 3.0 * (v_TNB * ShadingNormal));

        float DiffuseComponentHill = max(dot(ShadingNormalHill, u_lightDir), 0.0);
        float DiffuseComponentLand = max(dot(ShadingNormalLand, u_lightDir), 0.0);
        float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));
        float diffuseComponent = mix(DiffuseComponentLand, DiffuseComponentHill, TerrainTypeMixClamped) * sunPositionAttenuation;

        ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
        ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

        if (u_shadowEnable)
        {
            int shadowMapIndex;
            vec3 projectedCoords;
            float luminosity;
            ext_calculateShadowMapIndexAndProjectedCoords(shadowMapIndex, projectedCoords);
            float bias = u_bias * (2.0 - 1.0 * dot(u_lightDir, ShadingNormalHill));
            if (shadowMapIndex == 0)
                luminosity = ext_calculateLuminosity5(shadowMapIndex, projectedCoords, bias);
            else
                luminosity = ext_calculateLuminosity3Lowp(shadowMapIndex, projectedCoords, bias);

            diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
            float specularOn = step(0.9, luminosity);
            specularColor = specularOn * v_SpecularComponent * sampledSpecular.rgb * sunPositionAttenuation;
            resultColor = ambientColor + diffuseColor + specularColor;
            o_FragColor = vec4(resultColor, sampledDiffuse.a);
            float desaturatingValue = mix(0.0,
                                          mix(MAX_DESATURATING_VALUE_LAND, MAX_DESATURATING_VALUE_HILL, TerrainTypeMixClamped),
                                          min(luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE));
            ext_desaturate(o_FragColor, desaturatingValue);
        }
        else
        {
            diffuseColor = sampledDiffuse.rgb * diffuseComponent;
            specularColor = v_SpecularComponent * sampledSpecular.rgb * sunPositionAttenuation;
            resultColor = ambientColor + diffuseColor + specularColor;
            o_FragColor = vec4(resultColor, sampledDiffuse.a);
        }

        createCanyonRings(o_FragColor,
                          ambientColor + diffuseColor,
                          MIN_CANYON_CIRCLE_HEIGHT,
                          u_maxHillHeight,
                          NUM_CANYON_CIRCLES,
                          4,
                          ShadingNormalHill.y);
    }
}
