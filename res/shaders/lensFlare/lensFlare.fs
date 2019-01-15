#version 450
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 o_FragColor;

flat in int v_TexIndex;

const int NUM_LENS_FLARES = 7;

uniform uint64_t  u_flares[NUM_LENS_FLARES];
uniform float     u_brightnessFlare;
uniform float     u_brightnessHalo;

void main()
{
    o_FragColor = texture(sampler2D(u_flares[v_TexIndex]), gl_PointCoord);
    float brightness = u_brightnessFlare;
    if (v_TexIndex == 0)
        brightness = u_brightnessHalo;
    o_FragColor *= brightness;
}
