#version 450

out vec4 o_FragColor;

in vec3 v_TexCoords;

uniform samplerCube u_skyboxColor[3];
uniform samplerCube u_skyboxNormals[2];
uniform int         u_type;
uniform vec3        u_lightDir;
uniform float       u_ambientDay;
uniform float       u_ambientNight;

const int SKYBOX_TYPE_HILLS_NEAR = 0;
const int SKYBOX_TYPE_HILLS_FAR =  1;
const int SKYBOX_TYPE_CLOUDS =     2;
const float MAX_DESATURATING_VALUE = 0.5;

@include desaturationFunc.ifs

void main()
{
    if (u_type == SKYBOX_TYPE_CLOUDS)
    {
        float sunPositionAmplification = mix(1.0, 0.0, clamp((u_lightDir.y + 0.2) * 4, 0.0, 1.0));
        vec4 sampledClouds = texture(u_skyboxColor[SKYBOX_TYPE_CLOUDS], v_TexCoords);
        o_FragColor = mix(vec4(0.0, 0.0, 0.0, sampledClouds.a * 2), sampledClouds, sunPositionAmplification);
    }
    else
    {
        float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));
        vec4 sampledDiffuse = texture(u_skyboxColor[u_type], v_TexCoords);
        vec4 sampledNormal = texture(u_skyboxNormals[u_type], v_TexCoords);
        vec3 normal = sampledNormal.rgb;
        normal.x = normal.x * 2 - 1.0;
        normal.z = normal.z * 2 - 1.0;

        @include shadingVariables.ifs

        float diffuseComponent = max(dot(normal, u_lightDir), 0.0) * sunPositionAttenuation;

        ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
        ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        resultColor = ambientColor + diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
        float desaturatingValue = mix(0.0,
                                      MAX_DESATURATING_VALUE,
                                      min(1.0 * sunPositionAttenuation, diffuseComponent + 0.5));
        ext_desaturate(o_FragColor, desaturatingValue);
    }
}
