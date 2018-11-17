#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 2) in vec3 i_normal;

uniform mat4        u_projectionView;
uniform sampler2D   u_normal_map;

out vec3 v_Normal;

void main()
{
    gl_Position = u_projectionView * i_pos;
    vec3 ShadingNormal = clamp((texture(u_normal_map, i_pos.xz * 0.125).xzy) * 2.2, vec3(0.0), vec3(1.0));
    ShadingNormal.xyz -= vec3(0.5);
    v_Normal = normalize(i_normal + 1.2 * ShadingNormal);
}
