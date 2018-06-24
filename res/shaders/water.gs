#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3  v_SkyboxCoords[];
in float v_DiffuseComponent[];
in float v_SpecularComponent[];
in int   v_visible[];
in vec3  v_FragPos[];

out vec3  vg_SkyboxCoords;
out float vg_DiffuseComponent;
out float vg_SpecularComponent;
out vec3  vg_FragPos;

void main()
{
    if (v_visible[0] == 1)
    {
        gl_Position = gl_in[0].gl_Position;
        vg_SkyboxCoords = v_SkyboxCoords[0];
        vg_DiffuseComponent = v_DiffuseComponent[0];
        vg_SpecularComponent = v_SpecularComponent[0];
        vg_FragPos = v_FragPos[0];
        EmitVertex();
    }
    if (v_visible[1] == 1)
    {
        gl_Position = gl_in[1].gl_Position;
        vg_SkyboxCoords = v_SkyboxCoords[1];
        vg_DiffuseComponent = v_DiffuseComponent[1];
        vg_SpecularComponent = v_SpecularComponent[1];
        vg_FragPos = v_FragPos[1];
        EmitVertex();
    }
    if (v_visible[2] == 1)
    {
        gl_Position = gl_in[2].gl_Position;
        vg_SkyboxCoords = v_SkyboxCoords[2];
        vg_DiffuseComponent = v_DiffuseComponent[2];
        vg_SpecularComponent = v_SpecularComponent[2];
        vg_FragPos = v_FragPos[2];
        EmitVertex();
    }
    EndPrimitive();
}
