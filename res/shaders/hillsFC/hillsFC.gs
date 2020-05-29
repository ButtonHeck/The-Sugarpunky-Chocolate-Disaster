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
    for( int vertexIndex = 0; vertexIndex < gl_in.length(); vertexIndex++ )
    {
        if( v_visible[vertexIndex] != 1 )
		{
            break; //no need to continue as we would not get a triangle without all three vertices visible
		}
        o_pos = v_pos[vertexIndex];
        o_texCoords = v_texCoords[vertexIndex];
        o_normal = v_normal[vertexIndex];
        o_tangent = v_tangent[vertexIndex];
        o_bitangent = v_bitangent[vertexIndex];
        EmitVertex();
    }
    EndPrimitive();
}
