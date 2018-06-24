#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2  v_TexCoords[];
in float v_PosHeight[];
in float v_DiffuseComponentHill[];
in float v_DiffuseComponentFlat[];
in float v_TextureFlatMixRatio[];
in float v_TextureHillMixRatio[];
in float v_SpecularComponent[];
in int   v_visible[];
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

void main()
{
    if (v_visible[0] == 1)
    {
        vg_DiffuseComponentHill = v_DiffuseComponentHill[0];
        vg_DiffuseComponentFlat = v_DiffuseComponentFlat[0];
        vg_SpecularComponent = v_SpecularComponent[0];
        gl_Position = gl_in[0].gl_Position;
        vg_TexCoords = v_TexCoords[0];
        vg_PosHeight = v_PosHeight[0];
        vg_TextureFlatMixRatio = v_TextureFlatMixRatio[0];
        vg_TextureHillMixRatio = v_TextureHillMixRatio[0];
        vg_Normal = v_Normal[0];
        vg_ProjectedCoords = v_ProjectedCoords[0];
        EmitVertex();
    }
    if (v_visible[1] == 1)
    {
        vg_DiffuseComponentHill = v_DiffuseComponentHill[1];
        vg_DiffuseComponentFlat = v_DiffuseComponentFlat[1];
        vg_SpecularComponent = v_SpecularComponent[1];
        gl_Position = gl_in[1].gl_Position;
        vg_TexCoords = v_TexCoords[1];
        vg_PosHeight = v_PosHeight[1];
        vg_TextureFlatMixRatio = v_TextureFlatMixRatio[1];
        vg_TextureHillMixRatio = v_TextureHillMixRatio[1];
        vg_Normal = v_Normal[1];
        vg_ProjectedCoords = v_ProjectedCoords[1];
        EmitVertex();
    }
    if (v_visible[2] == 1)
    {
        vg_DiffuseComponentHill = v_DiffuseComponentHill[2];
        vg_DiffuseComponentFlat = v_DiffuseComponentFlat[2];
        vg_SpecularComponent = v_SpecularComponent[2];
        gl_Position = gl_in[2].gl_Position;
        vg_TexCoords = v_TexCoords[2];
        vg_PosHeight = v_PosHeight[2];
        vg_TextureFlatMixRatio = v_TextureFlatMixRatio[2];
        vg_TextureHillMixRatio = v_TextureHillMixRatio[2];
        vg_Normal = v_Normal[2];
        vg_ProjectedCoords = v_ProjectedCoords[2];
        EmitVertex();
    }
    EndPrimitive();
}
