#version 450

layout (points) in;
layout (line_strip, max_vertices = 2) out;

in vec3 v_gColor[];
in vec3 v_gDirection[];

out vec3 v_fColor;

void main()
{
    v_fColor = v_gColor[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(v_gDirection[0], 0.0);
    EmitVertex();
    EndPrimitive();
}
