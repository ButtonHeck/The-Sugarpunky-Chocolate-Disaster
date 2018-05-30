#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;

uniform mat4        u_projectionView;
uniform vec3        u_lightDir;
uniform sampler2D   u_normal_map;
uniform int         u_mapDimension;

out vec2  v_TexCoords;
out vec3  v_FragPos;
out float v_DiffuseComponent;

void main()
{
    gl_Position = u_projectionView * vec4(i_pos, 1.0);
    v_FragPos = i_pos;
    v_TexCoords = i_texCoords;
    vec3 FlatNormal = texture(u_normal_map, vec2(v_FragPos.x / (u_mapDimension >> 2), v_FragPos.z / (u_mapDimension >> 2))).rgb;
    vec3 ShadingNormal = normalize(FlatNormal);

    //diffuse
    v_DiffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0);
}
