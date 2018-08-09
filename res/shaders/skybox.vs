#version 450

layout (location = 0) in vec3 i_pos;

out vec3 v_TexCoords;

uniform mat4 u_projectionView;

void main()
{
    v_TexCoords = i_pos;
    vec4 correctPosition = u_projectionView * vec4(i_pos, 1.0);
    gl_Position = correctPosition.xyww;
}
