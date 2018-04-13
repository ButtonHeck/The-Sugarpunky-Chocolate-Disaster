#version 450

layout (location = 0) in vec4 posTex;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(posTex.xy, 0.0, 1.0);
    TexCoords = posTex.zw;
}
