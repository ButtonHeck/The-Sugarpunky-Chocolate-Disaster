#version 450

layout (location = 0) in vec3 i_pos;

out vec3 v_TexCoords;

uniform mat4  u_projectionView;
uniform vec3  u_viewPosition;
uniform int   u_type;

const int SKYBOX_TYPE_HILLS_NEAR = 0;
const int SKYBOX_TYPE_HILLS_FAR =  1;
const int SKYBOX_TYPE_CLOUDS =     2;

void main()
{
    v_TexCoords = i_pos;
    vec3 newPos = i_pos;
    if (u_type != SKYBOX_TYPE_CLOUDS)
        newPos -= u_viewPosition * (0.004 + 0.004 * u_type);
    vec4 correctPosition = u_projectionView * vec4(newPos, 1.0);
    gl_Position = correctPosition.xyww;
}
