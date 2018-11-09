#version 450

in vec3 g_Color;

out vec4 o_FragColor;

void main()
{
    o_FragColor = vec4(g_Color, 1.0);
}

