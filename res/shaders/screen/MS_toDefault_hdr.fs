#version 450

out vec4 o_FragColor;

uniform sampler2D u_frameTexture;
uniform sampler2D u_frameDepthTexture;
uniform float     u_exposure;
uniform float     u_near;
uniform float     u_far;

in vec2 v_TexCoords;

#ifdef HDR_ENABLED
const float GAMMA = 1.4;
const float GAMMA_INVERSED = 1.0 / GAMMA;
#endif

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near));
}

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
