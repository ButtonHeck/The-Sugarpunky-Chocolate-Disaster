#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2  v_TexCoords[];
in vec3  v_SkyboxCoords[];
in float v_DiffuseComponent[];
in float v_SpecularComponent[];
in int   v_visible[];

out vec2  vg_TexCoords;
out vec3  vg_SkyboxCoords;
out float vg_DiffuseComponent;
out float vg_SpecularComponent;

void main()
{
    //diffuse and specular components are the same for each three vertices (cause Gouraud shading)
    //so we don't need to reassign it in every vertex
    vg_DiffuseComponent = v_DiffuseComponent[0];
    vg_SpecularComponent = v_SpecularComponent[0];
    if (v_visible[0] == 1)
    {
        gl_Position = gl_in[0].gl_Position;
        vg_TexCoords = v_TexCoords[0];
        vg_SkyboxCoords = v_SkyboxCoords[0];
        EmitVertex();
    }
    if (v_visible[1] == 1)
    {
        gl_Position = gl_in[1].gl_Position;
        vg_TexCoords = v_TexCoords[1];
        vg_SkyboxCoords = v_SkyboxCoords[1];
        EmitVertex();
    }
    if (v_visible[2] == 1)
    {
        gl_Position = gl_in[2].gl_Position;
        vg_TexCoords = v_TexCoords[2];
        vg_SkyboxCoords = v_SkyboxCoords[2];
        EmitVertex();
    }
    EndPrimitive();
}
