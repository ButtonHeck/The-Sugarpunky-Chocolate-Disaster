#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_texCoords;

uniform mat4        u_projectionView;

out vec2  v_TexCoords;
out vec2  v_FragPosXZ;

void main()
{
    gl_Position = u_projectionView * i_pos;
    v_FragPosXZ = i_pos.xz;
    v_TexCoords = i_texCoords;
}
