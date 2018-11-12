#version 450

out vec4 o_FragColor;

uniform sampler2D u_texture;

void main()
{
    o_FragColor = texture(u_texture, gl_PointCoord);
}
