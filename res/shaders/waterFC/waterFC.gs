#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 v_FragPos[];
in vec3 v_normal[];

out vec3 o_pos;
out vec3 o_normal;

uniform vec4 u_frustumPlanes[5];
const float CULLING_OFFSET = 1.4142; //sqrt(2)

bool frustumCulling(vec3 position)
{
    for (int planeIndex = 0; planeIndex < u_frustumPlanes.length(); planeIndex++)
    {
        if (dot(u_frustumPlanes[planeIndex].xyz, position) <= -u_frustumPlanes[planeIndex].w - CULLING_OFFSET)
          return false;
    }
  return true;
}

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        o_pos = v_FragPos[i];
        o_normal = v_normal[i];
        if (frustumCulling(v_FragPos[i]))
            EmitVertex();
    }
    EndPrimitive();
}
