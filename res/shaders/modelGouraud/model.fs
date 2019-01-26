#version 450
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 o_FragColor;

in vec2         v_TexCoords;
in float        v_DiffuseComponent;
in float        v_SpecularComponent;
in float        v_AlphaValue;
in float        v_SunPositionAttenuation;
in float        v_NormalY;
in vec3         v_FragPos;
flat in uvec2   v_TexIndices;

uniform uint64_t  u_texture_diffuse[200];
uniform uint64_t  u_texture_specular[150];
uniform bool      u_shadow;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
uniform bool      u_useLandBlending;
uniform float     u_ambientDay;
uniform float     u_ambientNight;
uniform vec3      u_viewPosition;
uniform bool      u_isLowPoly;

const float MAX_DESATURATING_VALUE = 0.5;

@include shadowSampling.ifs
@include desaturationFunc.ifs

void main()
{
    vec4 sampledDiffuse = texture(sampler2D(u_texture_diffuse[v_TexIndices.x]), v_TexCoords);
    vec4 sampledSpecular = sampledDiffuse * texture(sampler2D(u_texture_specular[v_TexIndices.y]), v_TexCoords).r;

    @include shadingVariables.ifs

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, v_SunPositionAttenuation);
    ambientColor += nightAmbientColor * (1.0 - v_SunPositionAttenuation);

    if (u_shadowEnable)
    {
        int shadowMapIndex;
        vec3 projectedCoords;
        float luminosity;
        if (!u_isLowPoly)
        {
            ext_calculateShadowMapIndexAndProjectedCoords(shadowMapIndex, projectedCoords);
            luminosity = ext_calculateLuminosity5(shadowMapIndex, projectedCoords, u_bias);
        }
        else
        {
            ext_calculateShadowMapIndexAndProjectedCoordsLowp(shadowMapIndex, projectedCoords);
            luminosity = ext_calculateLuminosity3Lowp(shadowMapIndex, projectedCoords, u_bias);
        }

        diffuseColor = luminosity * sampledDiffuse.rgb * v_DiffuseComponent * (1.0 - u_ambientDay);
        float specularLuminosityInfluence = 1.0 - ((1.0 - luminosity) * SHADOW_INFLUENCE_RECIPROCAL);
        specularColor = specularLuminosityInfluence * luminosity * sampledSpecular.rgb * v_SpecularComponent;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(luminosity * v_SunPositionAttenuation, v_DiffuseComponent + SHADOW_INFLUENCE));
        ext_desaturate(o_FragColor, desaturatingValue);
        o_FragColor += clamp(o_FragColor * v_NormalY * 0.5, -0.04, 0.0);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * v_DiffuseComponent;
        specularColor = sampledSpecular.rgb * v_SpecularComponent;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        o_FragColor += clamp(o_FragColor * v_NormalY * 0.5, -0.04, 0.0);
    }

    if(u_useLandBlending)
        o_FragColor.a = mix(0.0, 1.0, v_AlphaValue);
}
