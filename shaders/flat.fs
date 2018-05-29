#version 450

out vec4 o_FragColor;

in vec3 v_FragPos;
in vec2 v_TexCoords;
in vec3 v_LightDir;

uniform sampler2D u_flat_diffuse;
uniform sampler2D u_flat_diffuse2;
uniform sampler2D u_normal_map;
uniform int       u_mapDimension;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);

void main()
{
    vec3 FlatNormal = texture(u_normal_map, vec2(v_FragPos.x / u_mapDimension + 0.5, v_FragPos.z / u_mapDimension + 0.5)).rgb;
    vec4 sampledDiffuse = mix(texture(u_flat_diffuse, v_TexCoords), texture(u_flat_diffuse2, v_TexCoords), FlatNormal.r);
    vec3 ShadingNormal = normalize(NORMAL + FlatNormal);

    //diffuse
    float diffuseComponent = max(dot(ShadingNormal, v_LightDir), 0.0);
    vec3 diffuseColor = sampledDiffuse.rgb * (diffuseComponent * 0.4 + 0.6);

    vec3 resultColor = diffuseColor;
    o_FragColor = vec4(resultColor, sampledDiffuse.a);
}
