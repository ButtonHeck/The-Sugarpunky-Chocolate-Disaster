#version 450

layout (location = 0) in vec4 i_pos;

void main()
{
    vec4 pos = i_pos;
    //slightly change y coord for smoother shadows at surface edges
    pos.y -= 0.06;
    /*
    in addition if a fragment is lower than the ground level, make it even lower,
    this leads to effect of hills being solid instead of wireframe, which leads
    to correct shadowing for shore
    */
    if (pos.y < 0)
        pos.y -= 5.0;
    gl_Position = pos;
}
