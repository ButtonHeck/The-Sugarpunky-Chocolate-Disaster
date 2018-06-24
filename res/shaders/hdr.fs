#version 450

out vec4 o_FragColor;

uniform sampler2D u_frameTexture;
uniform float     u_exposure;

in vec2 v_TexCoords;

const float GAMMA = 1.4;
const float GAMMA_INVERSED = 1.0 / GAMMA;

void main()
{
    vec3 hdrColor = texture(u_frameTexture, v_TexCoords).rgb;

    //exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * u_exposure);

    //gamma correction
    mapped = pow(mapped, vec3(GAMMA_INVERSED));
    o_FragColor = vec4(mapped, 1.0);
}
