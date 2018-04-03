#version 450

layout (points) in;
layout (line_strip, max_vertices = 2) out;

in vec3 gColor[];
in vec3 gDirection[];

uniform mat4 projection;

out vec3 fColor;

void main()
{
    fColor = gColor[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(gDirection[0], 0.0);
    EmitVertex();
    EndPrimitive();
}
