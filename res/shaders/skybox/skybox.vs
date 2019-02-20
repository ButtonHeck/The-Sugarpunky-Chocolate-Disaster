#version 450

layout (location = 0) in vec3 i_pos;

out vec3 v_TexCoords;

uniform mat4  u_projectionView;
uniform vec3  u_viewPosition;
//u_type - defines what kind of skybox is being rendered
uniform int   u_type;

const int   SKYBOX_TYPE_HILLS_NEAR = 0;
const int   SKYBOX_TYPE_HILLS_FAR =  1;
const int   SKYBOX_TYPE_CLOUDS =     2;
const float PARALLAX_SCROLLING_OFFSET = 0.001;

void main()
{
    v_TexCoords = i_pos;
    vec3 newPos = i_pos;

    //apply some parallax to skybox position depending on the view position
    if (u_type != SKYBOX_TYPE_CLOUDS)
        newPos -= u_viewPosition * (PARALLAX_SCROLLING_OFFSET + u_type * PARALLAX_SCROLLING_OFFSET);

    vec4 correctPosition = u_projectionView * vec4(newPos, 1.0);
    //use W as Z, so after perspective division we get Z as 1.0
    gl_Position = correctPosition.xyww;
}
