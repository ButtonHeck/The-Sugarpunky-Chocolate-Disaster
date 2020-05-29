/*
this shader is used only for visual debug purposes
and is to be cut out from the project in release time
*/

#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 2) in vec3 i_normal;
layout (location = 3) in vec3 i_tangent;
layout (location = 4) in vec3 i_bitangent;

uniform mat4        u_projectionView;
uniform sampler2D   u_normalMap;

out vec3 v_Normal;
out vec3 v_Tangent;
out vec3 v_Bitangent;

void main()
{
    gl_Position = u_projectionView * i_pos;
    vec3 ShadingNormal = texture( u_normalMap, i_pos.xz * 0.125 ).xzy;
    ShadingNormal.xyz -= vec3(0.5);
    v_Normal = normalize( i_normal + ShadingNormal );
    v_Tangent = normalize(i_tangent);
    v_Bitangent = normalize(i_bitangent);
}
