#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_texCoords;

uniform mat4        u_projectionView;
uniform vec3        u_lightDir;
uniform sampler2D   u_normal_map;
uniform float       u_mapDimension;

out vec2  v_TexCoords;
out vec2  v_FragPosXZ;
out float v_DiffuseComponent;

void main()
{
    gl_Position = u_projectionView * i_pos;
    v_FragPosXZ = i_pos.xz;
    v_TexCoords = i_texCoords;
    vec3 FlatNormal = texture(u_normal_map, v_FragPosXZ * u_mapDimension + 0.5).rgb;
    vec3 ShadingNormal = normalize(FlatNormal);

    //diffuse
    v_DiffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0);
}
