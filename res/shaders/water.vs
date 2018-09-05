#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;

uniform mat4 u_projectionView;
uniform vec3 u_lightDir;
uniform vec3 u_viewPosition;

out vec3  v_FragPos;
out vec3  v_Normal;
out vec3  v_ViewDir;

void main()
{
    gl_Position = u_projectionView * vec4(i_pos, 1.0);
    v_FragPos = i_pos;
    v_Normal = i_normal;
    v_ViewDir = normalize(u_viewPosition - i_pos);
}
