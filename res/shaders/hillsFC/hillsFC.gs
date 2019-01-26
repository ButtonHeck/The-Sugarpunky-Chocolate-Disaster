#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 v_pos[];
in vec2 v_texCoords[];
in vec3 v_normal[];
in vec3 v_tangent[];
in vec3 v_bitangent[];
in int  v_visible[];

out vec3 o_pos;
out vec2 o_texCoords;
out vec3 o_normal;
out vec3 o_tangent;
out vec3 o_bitangent;

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        if (v_visible[i] != 1)
            break;
        o_pos = v_pos[i];
        o_texCoords = v_texCoords[i];
        o_normal = v_normal[i];
        o_tangent = v_tangent[i];
        o_bitangent = v_bitangent[i];
        EmitVertex();
    }
    EndPrimitive();
}
