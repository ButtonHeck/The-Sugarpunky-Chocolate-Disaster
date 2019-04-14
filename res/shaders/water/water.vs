#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;

uniform mat4  u_projectionView;
uniform float u_time;

out vec3  v_FragPos;
out vec3  v_Normal;

void main()
{
    vec3 animatedPos = i_pos;
    animatedPos.y += sin(u_time * (animatedPos.x * 0.311 + animatedPos.z * 0.131) * 0.21) * 0.0825;
    gl_Position = u_projectionView * vec4(animatedPos, 1.0);
    v_FragPos = animatedPos;
    v_Normal = i_normal;
}
