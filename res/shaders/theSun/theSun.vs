#version 450

layout (location = 0) in vec4 i_pos;

uniform mat4 u_model;
uniform mat4 u_projectionView;

//v_PositionY - used for interpolate from pink color to texture during dusk/dawn
out float v_PositionY;

void main()
{
    //use W as Z, so after perspective division we get Z as 1.0
    gl_Position = ( u_projectionView * u_model * i_pos ).xyww;
    v_PositionY = u_model[0][1];
}
