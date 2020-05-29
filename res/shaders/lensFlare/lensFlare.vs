#version 450

layout (location = 0) in vec2  i_pos;
layout (location = 1) in float i_pointSize;

out int v_TexIndex;

void main()
{
    vec2 NDCcoords = i_pos;
    //map from screen texture coordinates to actual NDC space
    NDCcoords *= 2.0;
    NDCcoords.y = 1.0 - NDCcoords.y;
    NDCcoords.x = NDCcoords.x - 1.0;
    gl_PointSize = i_pointSize;
    //keep Z component as -1.0 because a flare should not be occluded by anything
    gl_Position = vec4( NDCcoords, -1.0, 1.0 );

    //flares are being rendered in one draw call, so each vertex define its own index in a texture array
    v_TexIndex = gl_VertexID;
}
