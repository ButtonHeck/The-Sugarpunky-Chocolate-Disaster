#version 450

out vec4 o_FragColor;

flat in int v_TexIndex;

const int NUM_LENS_FLARES = 7;

uniform sampler2D u_flares[NUM_LENS_FLARES];
uniform float     u_brightnessFlare;
uniform float     u_brightnessHalo;

void main()
{
    o_FragColor = texture(u_flares[v_TexIndex], gl_PointCoord);
    float brightness = u_brightnessFlare;
    if (v_TexIndex == 0)
        brightness = u_brightnessHalo;
    o_FragColor *= brightness;
}
