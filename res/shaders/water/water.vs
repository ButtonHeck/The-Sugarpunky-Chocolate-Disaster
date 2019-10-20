#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 0, component = 3) in float i_animationOffset;

uniform mat4  u_projectionView;
uniform float u_time;

out vec3  v_FragPos;

const float WAVE_AMPLITUDE = 0.0825;

void main()
{
    vec3 animatedPos = i_pos;
    animatedPos.y += sin(u_time + i_animationOffset) * WAVE_AMPLITUDE;
    gl_Position = u_projectionView * vec4(animatedPos, 1.0);
    v_FragPos = animatedPos;
}
