#version 450

layout (location = 0, component = 0) in vec2 i_pos;
layout (location = 0, component = 2) in vec2 i_texCoords;

out vec2 v_TexCoords;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4( i_pos, 0.0, 1.0 );
    //explicitly set Z to -0.99 as it should not overlay with lens flares but always be in front of other objects
    gl_Position.z = -0.99;
    v_TexCoords = i_texCoords;
}
