#version 450

out vec4 o_FragColor;

in vec3 v_TexCoords;

uniform samplerCube u_skybox[3];
uniform int u_index;

void main()
{
    o_FragColor = texture(u_skybox[u_index], v_TexCoords);
}
