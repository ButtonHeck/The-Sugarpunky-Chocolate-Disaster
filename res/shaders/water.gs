#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3  v_SkyboxCoords[];
in float v_DiffuseComponent[];
in float v_SpecularComponent[];
in vec3  v_FragPos[];

out vec3  vg_SkyboxCoords;
out float vg_DiffuseComponent;
out float vg_SpecularComponent;
out vec3  vg_FragPos;

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
        vg_SkyboxCoords = v_SkyboxCoords[i];
        vg_DiffuseComponent = v_DiffuseComponent[i];
        vg_SpecularComponent = v_SpecularComponent[i];
        vg_FragPos = v_FragPos[i];
        if (frustumCulling(v_FragPos[i]))
            EmitVertex();
    }
    EndPrimitive();
}
