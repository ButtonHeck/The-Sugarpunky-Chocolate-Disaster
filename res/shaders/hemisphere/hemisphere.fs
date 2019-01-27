#version 450

out vec4 o_FragColor;

uniform sampler2D u_hemisphereDiffuse;
uniform vec3      u_lightDir;

in vec2 v_TexCoords;
in vec3 v_Normal;

void main()
{
    vec3 normal = normalize(v_Normal);
    float sunPositionAttenuation = mix(0.0, 1.0, clamp((u_lightDir.y + 0.05) * 8, 0.0, 1.0));
    float normalInfluence = mix(pow((1.0 - normal.y), 1.2), max(dot(u_lightDir, normal), 0.0), sunPositionAttenuation);
    o_FragColor = texture(u_hemisphereDiffuse, v_TexCoords) * normalInfluence * sunPositionAttenuation;
}
