#version 450

out vec4 o_FragColor;

in vec3 v_TexCoords;

uniform samplerCube u_skybox[3];
uniform samplerCube u_skyboxNormals[2];
uniform int         u_index;
uniform vec3        u_lightDir;
uniform bool        u_static;
uniform float       u_ambientDay;
uniform float       u_ambientNight;

void main()
{
    if (u_static)
        o_FragColor = texture(u_skybox[u_index], v_TexCoords);
    else
    {
        vec4 sampledDiffuse = texture(u_skybox[u_index], v_TexCoords);
        vec4 sampledNormal = texture(u_skyboxNormals[u_index], v_TexCoords);
        vec3 normal = sampledNormal.rgb;
        normal.x = normal.x * 2 - 1.0;
        normal.z = normal.z * 2 - 1.0;

        @include shadingVariables.ifs

        float sunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 10, 0.0, 1.0));
        float diffuseComponent = max(dot(normal, u_lightDir), 0.0) * sunPositionAttenuation;

        ambientColor = mix(ambientColorNightSelf, ambientColorDaySelf, sunPositionAttenuation);
        ambientColor += nightAmbientColor * (1.0 - sunPositionAttenuation);

        diffuseColor = sampledDiffuse.rgb * diffuseComponent;
        resultColor = ambientColor + diffuseColor;
        o_FragColor = vec4(resultColor, sampledDiffuse.a);
    }
}
