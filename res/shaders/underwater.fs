#version 450

out vec4 o_FragColor;

in vec3  v_FragPos;
in vec2  v_TexCoords;
in float v_DiffuseComponent;

uniform sampler2D u_underwater_diffuse;
uniform sampler2D u_bottomRelief_diffuse;
uniform int       u_mapDimension;

void main()
{
    float reliefAttenuation = 1.0 - texture(u_bottomRelief_diffuse, vec2(v_FragPos.x / u_mapDimension + 0.5, v_FragPos.z / u_mapDimension + 0.5)).r * 1.1;
    vec4 sampledDiffuse = texture(u_underwater_diffuse, v_TexCoords);
    vec3 diffuseColor = v_DiffuseComponent * sampledDiffuse.rgb * reliefAttenuation;
    o_FragColor = vec4(diffuseColor, sampledDiffuse.a);
}
