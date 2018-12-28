#version 450
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 o_FragColor;

in vec2         v_TexCoords;
in vec3         v_Normal;
in float        v_AlphaValue;
in vec3         v_FragPos;
flat in uvec2   v_TexIndices;

uniform uint64_t  u_texture_diffuse[150];
uniform uint64_t  u_texture_specular[100];
uniform bool      u_shadow;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
uniform bool      u_useLandBlending;
uniform bool      u_isGrass;
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

    float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 5, 0.0, 1.0));
    vec3 shadingNormal = normalize(v_Normal);
    if (u_isGrass)
        shadingNormal.y *= sign(shadingNormal.y) * mix(1.0, u_lightDir.y, sunPositionAttenuation); //intentionally left unnormalized

    float diffuseComponent = max(dot(shadingNormal, u_lightDir), 0.0) * sunPositionAttenuation * (1.0 - u_ambientDay);

    vec3 Reflect = reflect(-u_lightDir, shadingNormal);
    vec3 ViewDir = normalize(u_viewPosition - v_FragPos);
    float specularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 4.0) * sunPositionAttenuation;

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
    ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

    if (u_shadowEnable)
    {
        int shadowMapIndex;
        vec3 projectedCoords;
        float luminosity;
        ext_calculateShadowMapIndexAndProjectedCoords(shadowMapIndex, projectedCoords);
        if (!u_isLowPoly)
            luminosity = ext_calculateLuminosity5(shadowMapIndex, projectedCoords, u_bias);
        else
            luminosity = ext_calculateLuminosity3(shadowMapIndex, projectedCoords, u_bias);

        diffuseColor = luminosity * sampledDiffuse.rgb * diffuseComponent;
        specularColor = luminosity * specularComponent * sampledSpecular.rgb;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(luminosity * sunPositionAttenuation, diffuseComponent + SHADOW_INFLUENCE));
        ext_desaturate(o_FragColor, desaturatingValue);
        o_FragColor += clamp(o_FragColor * shadingNormal.y * 0.5, -0.04, 0.0);
    }
    else
    {
        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        specularColor = sampledSpecular.rgb * specularComponent;
        resultColor = ambientColor + diffuseColor + specularColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        o_FragColor += clamp(o_FragColor * shadingNormal.y * 0.5, -0.04, 0.0);
    }

    if(u_useLandBlending)
        o_FragColor.a = mix(0.0, 1.0, v_AlphaValue);
}
