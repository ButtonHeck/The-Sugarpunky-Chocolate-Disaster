#version 450

layout (location = 0) in vec3 i_pos;

uniform mat4 u_projectionView;
uniform bool u_isVolume;

void main()
{
    gl_Position = u_projectionView * vec4(i_pos, 1.0);
    if (u_isVolume)
        gl_Position.z = 0.0;
}
