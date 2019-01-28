#version 450

in vec2 v_TexCoords;
out vec4 o_FragColor;

uniform sampler2D u_frameTexture;
uniform sampler2D u_frameDepthTexture;
uniform sampler2D u_vignetteTexture;
uniform float     u_exposure;
uniform float     u_near;
uniform float     u_far;
uniform float     u_aspectRatio;
uniform bool      u_useDOF;

#ifdef HDR_ENABLED
const float GAMMA = 1.4;
const float GAMMA_INVERSED = 1.0 / GAMMA;
#endif

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near));
}

const float BLUR_SIZE_H = 1.0 / 1400.0;
const float BLUR_SIZE_V = BLUR_SIZE_H / u_aspectRatio;

void blur(inout vec3 fragColor, int coordOffset, float depthClip)
{
    vec3 sum = vec3(0.0);
    int divisor = 0;
    for (int x = -coordOffset; x <= coordOffset; x++)
    {
        for (int y = -coordOffset; y <= coordOffset; y++)
        {
            vec2 texOffset = vec2(v_TexCoords.x + x * BLUR_SIZE_H, v_TexCoords.y + y * BLUR_SIZE_V);
            float currentDepth = texture(u_frameDepthTexture, texOffset).r;
            //currentDepth = linearizeDepth(currentDepth) / u_far;
            if (currentDepth > depthClip && currentDepth < 0.999999)
            {
                sum += texture(u_frameTexture, texOffset).rgb;
                ++divisor;
            }
        }
    }
    sum /= divisor;
    fragColor = sum;
}

const float SOBEL_SIZE_H = 1.0 / 4000.0;
const float SOBEL_SIZE_V = SOBEL_SIZE_H / u_aspectRatio;

void sobel(inout vec3 fragColor)
{
    vec4 top         = texture(u_frameTexture, vec2(v_TexCoords.x, v_TexCoords.y + SOBEL_SIZE_V));
    vec4 bottom      = texture(u_frameTexture, vec2(v_TexCoords.x, v_TexCoords.y - SOBEL_SIZE_V));
    vec4 left        = texture(u_frameTexture, vec2(v_TexCoords.x - SOBEL_SIZE_H, v_TexCoords.y));
    vec4 right       = texture(u_frameTexture, vec2(v_TexCoords.x + SOBEL_SIZE_H, v_TexCoords.y));
    vec4 topLeft     = texture(u_frameTexture, vec2(v_TexCoords.x - SOBEL_SIZE_H, v_TexCoords.y + SOBEL_SIZE_V));
    vec4 topRight    = texture(u_frameTexture, vec2(v_TexCoords.x + SOBEL_SIZE_H, v_TexCoords.y + SOBEL_SIZE_V));
    vec4 bottomLeft  = texture(u_frameTexture, vec2(v_TexCoords.x - SOBEL_SIZE_H, v_TexCoords.y - SOBEL_SIZE_V));
    vec4 bottomRight = texture(u_frameTexture, vec2(v_TexCoords.x + SOBEL_SIZE_H, v_TexCoords.y - SOBEL_SIZE_V));
    vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
    vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
    vec4 sobel = sqrt(sx * sx + sy * sy);
    fragColor = sobel.rgb;
}

void main()
{
    vec3 color = texture(u_frameTexture, v_TexCoords).rgb;
    if (u_useDOF)
    {
        float depthValue = texture(u_frameDepthTexture, v_TexCoords).r;
        //depthValue = linearizeDepth(depthValue) / u_far;
        if (depthValue > 0.997 && depthValue < 0.999999)
            blur(color, 1, 0.997);
    }

    #ifdef HDR_ENABLED
    {
        vec3 mapped = vec3(1.0) - exp(-color * u_exposure);
        mapped = pow(mapped, vec3(GAMMA_INVERSED));
        o_FragColor = vec4(mapped, 1.0);
    }
    #else
        o_FragColor = vec4(color, 1.0);
    #endif
    float vignetteSampledAlpha = texture(u_vignetteTexture, v_TexCoords).a;
    o_FragColor.gb *= 1.0 - vec2(pow(vignetteSampledAlpha, 5.0), pow(vignetteSampledAlpha, 4.0));
}
