/*
this shader is used only for visual debug purposes
and is to be cut out from the project in release time
*/

#version 450

layout (points) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 u_projectionView;

in vec3 v_Normal[];
in vec3 v_Tangent[];
in vec3 v_Bitangent[];

out vec3 v_fColor;

void main()
{
    v_fColor = vec3( abs( v_Normal[0].x ) * 1.5, 0.0, abs( v_Normal[0] ).z * 1.5 );
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + u_projectionView * vec4( v_Normal[0], 0.0 );
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + u_projectionView * vec4( v_Tangent[0], 0.0 );
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + u_projectionView * vec4( v_Bitangent[0], 0.0 );
    EmitVertex();

    EndPrimitive();
}
