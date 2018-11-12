#version 450

out vec4 o_FragColor;

uniform sampler2D u_frameTexture;
uniform float     u_exposure;

in vec2 v_TexCoords;

#ifdef HDR_ENABLED
const float GAMMA = 1.4;
const float GAMMA_INVERSED = 1.0 / GAMMA;
#endif

void main()
{
    vec3 color = texture(u_frameTexture, v_TexCoords).rgb;
    #ifdef HDR_ENABLED
    {
        vec3 mapped = vec3(1.0) - exp(-color * u_exposure);
        mapped = pow(mapped, vec3(GAMMA_INVERSED));
        o_FragColor = vec4(mapped, 1.0);
    }
    #else
        o_FragColor = vec4(color, 1.0);
    #endif
}
