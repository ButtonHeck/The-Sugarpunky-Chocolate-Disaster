#version 450

layout (location = 0) in vec3 i_pos;

uniform mat4  u_projectionView;
uniform float u_time;

out vec3  v_FragPos;

const float HEIGHT_OFFSET = 0.0825;
const float X_POS_ANIM_MULTIPLIER = 15.11;
const float X_POS_ANIM_OFFSET = 121.197;

void main()
{
    vec3 animatedPos = i_pos;
    animatedPos.y += sin(u_time + animatedPos.x * X_POS_ANIM_MULTIPLIER
                         +
                         animatedPos.z * (X_POS_ANIM_OFFSET + animatedPos.x))
                     * HEIGHT_OFFSET;
    gl_Position = u_projectionView * vec4(animatedPos, 1.0);
    v_FragPos = animatedPos;
}
