#version 450

const int NUM_LAYERS = 3;

layout (triangles) in;
layout (triangle_strip, max_vertices = NUM_LAYERS * 3) out;

uniform mat4 u_lightSpaceMatrix[NUM_LAYERS];

void main()
{
    /*
    this shader is used during layered rendering for related framebuffer,
    so for each layer we should calculate different set of data
    */
    int layer, vertex;
    for (layer = 0; layer < NUM_LAYERS; layer++)
    {
        gl_Layer = layer;
        for (vertex = 0; vertex < gl_in.length(); vertex++)
        {
            gl_Position = u_lightSpaceMatrix[layer] * gl_in[vertex].gl_Position;
            //questionable(but working) trick to mimic hills be solid in order to remove shadow artefacts
            if (gl_Position.z < -1.0)
                gl_Position.z = -1.0;
            EmitVertex();
        }
        EndPrimitive();
    }
}
