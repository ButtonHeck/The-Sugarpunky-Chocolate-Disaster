#version 450

out vec4 o_FragColor;

in vec3 v_TexCoords;

uniform samplerCube u_skybox;

void main()
{
    o_FragColor = texture(u_skybox, v_TexCoords);
}
