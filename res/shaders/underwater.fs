#version 450

out vec4 o_FragColor;

in vec2  v_FragPosXZ;
in vec2  v_TexCoords;

uniform sampler2D u_underwater_diffuse;
uniform sampler2D u_bottomRelief_diffuse;
uniform sampler2D u_normal_map;
uniform float     u_mapDimension;
uniform vec3      u_lightDir;

void main()
{
    //swizzle z and y to rotate Z-aligned normal map 90 degrees around X axis, as like we look at it upside down
    //also scale up texture mapping a bit
    vec3 ShadingNormal = normalize(texture(u_normal_map, v_FragPosXZ * 0.0625).xzy);
    float diffuseComponent = dot(ShadingNormal, u_lightDir) * 0.5 + 0.5;

    float reliefAttenuation = 1.0 - texture(u_bottomRelief_diffuse, v_FragPosXZ * u_mapDimension + 0.5).r * 1.1;
    vec4 sampledDiffuse = texture(u_underwater_diffuse, v_TexCoords);
    sampledDiffuse.g -= (reliefAttenuation * 0.025 + 0.025);
    vec3 diffuseColor = diffuseComponent * sampledDiffuse.rgb * reliefAttenuation;
    o_FragColor = vec4(diffuseColor, sampledDiffuse.a);
}
