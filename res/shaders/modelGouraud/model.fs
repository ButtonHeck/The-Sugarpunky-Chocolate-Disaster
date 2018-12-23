#version 450
#extension GL_ARB_bindless_texture : enable

out vec4 o_FragColor;

in vec2         v_TexCoords;
in float        v_DiffuseComponent;
in float        v_SpecularComponent;
in float        v_AlphaValue;
in float        v_SunPositionAttenuation;
in float        v_NormalY;
in vec3         v_FragPos;
flat in uvec2   v_TexIndices;

layout (bindless_sampler) uniform;
//TODO: Now all the samplers are limited to 255 units, better keep these as uvec2 and pass through SSBO
uniform sampler2D u_texture_diffuse[150];
uniform sampler2D u_texture_specular[100];
uniform bool      u_shadow;
uniform vec3      u_lightDir;
uniform bool      u_shadowEnable;
uniform bool      u_useLandBlending;
uniform float     u_ambientDay;
uniform float     u_ambientNight;
uniform vec3      u_viewPosition;

const float MAX_DESATURATING_VALUE = 0.5;

@include shadowSampling.ifs
@include desaturationFunc.ifs

void main()
{
    vec4 sampledDiffuse = texture(u_texture_diffuse[v_TexIndices.x], v_TexCoords);
    vec4 sampledSpecular = sampledDiffuse * texture(u_texture_specular[v_TexIndices.y], v_TexCoords).r;

    @include shadingVariables.ifs

    ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, v_SunPositionAttenuation);
    ambientColor += nightAmbientColor * (1.0 - v_SunPositionAttenuation);

    if (u_shadowEnable)
    {
        int shadowMapIndex;
        vec3 projectedCoords;
        ext_calculateShadowMapIndexAndProjectedCoords(shadowMapIndex, projectedCoords);
        float luminosity = ext_calculateLuminosity(shadowMapIndex, projectedCoords);

        diffuseColor = luminosity * sampledDiffuse.rgb * v_DiffuseComponent;
        specularColor = luminosity * sampledSpecular.rgb * v_SpecularComponent;
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
