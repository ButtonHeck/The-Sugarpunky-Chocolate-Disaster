#version 450

layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 u_projectionView;

in vec3 v_Normal[];

out vec3 g_Color;

void main()
{
    g_Color = vec3(abs(v_Normal[0].x) * 1.5, 0.0, abs(v_Normal[0]).z * 1.5);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + u_projectionView * vec4(v_Normal[0], 0.0);
    EmitVertex();
    EndPrimitive();
}
