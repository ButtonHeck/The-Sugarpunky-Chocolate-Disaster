#version 450

layout (location = 0) in vec3 i_pos;

out vec3 v_TexCoords;

uniform mat4  u_projectionView;
uniform float u_cameraY;
uniform int   u_index;

void main()
{
    v_TexCoords = i_pos;
    vec3 newPos = i_pos;

    newPos.y -= ((u_cameraY - 12.0) * (0.005 + 0.0075 * u_index));
    vec4 correctPosition = u_projectionView * vec4(newPos, 1.0);
    gl_Position = correctPosition.xyww;
}
