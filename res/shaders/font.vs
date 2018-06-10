#version 450

layout (location = 0) in vec4 i_posTex;

out vec2 v_TexCoords;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(i_posTex.xy, 0.0, 1.0);
    v_TexCoords = i_posTex.zw;
}
