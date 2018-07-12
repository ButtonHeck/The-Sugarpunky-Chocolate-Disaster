#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3  v_FragPos[];
in vec2  v_TexCoords[];
in float v_PosHeight[];
in float v_DiffuseComponentHill[];
in float v_DiffuseComponentFlat[];
in float v_TextureFlatMixRatio[];
in float v_TextureHillMixRatio[];
in float v_SpecularComponent[];
in vec3  v_Normal[];
in vec3  v_ProjectedCoords[];

out vec2  vg_TexCoords;
out float vg_PosHeight;
out float vg_DiffuseComponentHill;
out float vg_DiffuseComponentFlat;
out float vg_TextureFlatMixRatio;
out float vg_TextureHillMixRatio;
out float vg_SpecularComponent;
out vec3  vg_Normal;
out vec3  vg_ProjectedCoords;

uniform vec4 u_frustumPlanes[5];
const float TILE_RADIUS = 2;

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
        vg_DiffuseComponentHill = v_DiffuseComponentHill[i];
        vg_DiffuseComponentFlat = v_DiffuseComponentFlat[i];
        vg_SpecularComponent = v_SpecularComponent[i];
        gl_Position = gl_in[i].gl_Position;
        vg_TexCoords = v_TexCoords[i];
        vg_PosHeight = v_PosHeight[i];
        vg_TextureFlatMixRatio = v_TextureFlatMixRatio[i];
        vg_TextureHillMixRatio = v_TextureHillMixRatio[i];
        vg_Normal = v_Normal[i];
        vg_ProjectedCoords = v_ProjectedCoords[i];
        if (frustumCulling(v_FragPos[i]))
            EmitVertex();
    }
    EndPrimitive();
}
