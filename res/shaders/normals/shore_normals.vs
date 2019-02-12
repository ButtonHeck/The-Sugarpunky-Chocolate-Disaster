#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 2) in vec3 i_normal;

uniform mat4        u_projectionView;
uniform sampler2D   u_normalMap;

out vec3 v_Normal;

void main()
{
    gl_Position = u_projectionView * i_pos;
    vec3 ShadingNormal = texture(u_normalMap, i_pos.xz * 0.125).xzy;
    ShadingNormal.xyz -= vec3(0.5);
    v_Normal = normalize(i_normal + 0.5 * ShadingNormal);
}
