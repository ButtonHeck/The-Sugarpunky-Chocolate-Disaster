#version 450

layout (location = 0) in vec2  i_pos;
layout (location = 1) in float i_pointSize;

out int v_TexIndex;

void main()
{
    vec2 NDCcoords = i_pos;
    NDCcoords *= 2.0;
    NDCcoords.y = 1.0 - NDCcoords.y;
    NDCcoords.x = NDCcoords.x - 1.0;
    gl_PointSize = i_pointSize;
    gl_Position = vec4(NDCcoords, 0.0, 1.0);
    v_TexIndex = gl_VertexID;
}
