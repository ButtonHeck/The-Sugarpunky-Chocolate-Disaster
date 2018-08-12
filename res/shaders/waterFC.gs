#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 v_FragPos[];
in vec3 v_normal[];

out vec3 o_pos;
out vec3 o_normal;

uniform vec4 u_frustumPlanes[5];
const float TILE_RADIUS = sqrt(2);

bool frustumCulling(vec3 position)
{
    for (int i = 0; i < u_frustumPlanes.length(); i++)
    {
        if (dot(u_frustumPlanes[i].xyz, position) <= -u_frustumPlanes[i].w - TILE_RADIUS)
          return false;
    }
  return true;
}

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        o_pos = v_FragPos[i];
        o_normal = v_normal[i];
        if (frustumCulling(v_FragPos[i]))
            EmitVertex();
    }
    EndPrimitive();
}
