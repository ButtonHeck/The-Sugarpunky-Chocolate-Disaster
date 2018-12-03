#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

uniform mat4 u_lightSpaceMatrix[2];

void main()
{
    int layer, i;
    for (layer = 0; layer < 2; layer++)
    {
        gl_Layer = layer;
        for (i = 0; i < gl_in.length(); i++)
        {
            gl_Position = u_lightSpaceMatrix[layer] * gl_in[i].gl_Position;
            if (gl_Position.z < -1.0)
                gl_Position.z = -1.0;
            EmitVertex();
        }
        EndPrimitive();
    }
}
