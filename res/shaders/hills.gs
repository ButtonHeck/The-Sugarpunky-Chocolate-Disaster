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

out vec2  vg_TexCoords;
out float vg_PosHeight;
out float vg_DiffuseComponentHill;
out float vg_DiffuseComponentFlat;
out float vg_TextureFlatMixRatio;
out float vg_TextureHillMixRatio;
out float vg_SpecularComponent;

void main()
{
    vg_DiffuseComponentHill = v_DiffuseComponentHill[0];
    vg_DiffuseComponentFlat = v_DiffuseComponentFlat[0];
    vg_SpecularComponent = v_SpecularComponent[0];
    if (v_visible[0] == 1)
    {
        gl_Position = gl_in[0].gl_Position;
        vg_TexCoords = v_TexCoords[0];
        vg_PosHeight = v_PosHeight[0];
        vg_TextureFlatMixRatio = v_TextureFlatMixRatio[0];
        vg_TextureHillMixRatio = v_TextureHillMixRatio[0];
        EmitVertex();
    }
    if (v_visible[1] == 1)
    {
        gl_Position = gl_in[1].gl_Position;
        vg_TexCoords = v_TexCoords[1];
        vg_PosHeight = v_PosHeight[1];
        vg_TextureFlatMixRatio = v_TextureFlatMixRatio[1];
        vg_TextureHillMixRatio = v_TextureHillMixRatio[1];
        EmitVertex();
    }
    if (v_visible[2] == 1)
    {
        gl_Position = gl_in[2].gl_Position;
        vg_TexCoords = v_TexCoords[2];
        vg_PosHeight = v_PosHeight[2];
        vg_TextureFlatMixRatio = v_TextureFlatMixRatio[2];
        vg_TextureHillMixRatio = v_TextureHillMixRatio[2];
        EmitVertex();
    }
    EndPrimitive();
}
