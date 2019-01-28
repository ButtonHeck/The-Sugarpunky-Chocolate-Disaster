#version 450

out vec4 o_FragColor;

uniform sampler2D u_theSunAmbientLightingDiffuse;
uniform sampler2D u_starsDiffuse;
uniform vec3      u_lightDir;
uniform int       u_type;

const int SKYSPHERE_TYPE_AMBIENT_LIGHTING = 0;
const int SKYSPHERE_TYPE_STARS = 1;

in vec2 v_TexCoords;
in vec3 v_Normal;

void main()
{
    vec3 normal = normalize(v_Normal);
    float sunPositionAttenuation = mix(0.0, 1.0, clamp((u_lightDir.y + 0.05) * 8, 0.0, 1.0));
    float normalInfluence = mix(pow((1.0 - normal.y), 1.2), max(dot(u_lightDir, normal), 0.0), sunPositionAttenuation);
    if (u_type == SKYSPHERE_TYPE_AMBIENT_LIGHTING)
        o_FragColor = texture(u_theSunAmbientLightingDiffuse, v_TexCoords) * normalInfluence * sunPositionAttenuation;
    else
    {
        float textureMix = pow(abs(0.5 - v_TexCoords.y) + 0.5, 8.0);
        o_FragColor = pow(mix(texture(u_starsDiffuse, v_TexCoords), vec4(0.0), textureMix), vec4(1.6))
                      * (1.0 - sunPositionAttenuation);
    }
}
