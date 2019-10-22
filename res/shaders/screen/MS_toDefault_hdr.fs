#version 450

in vec2 v_TexCoords;
out vec4 o_FragColor;

uniform sampler2D u_frameTexture;
uniform sampler2D u_frameDepthTexture;
uniform sampler2D u_vignetteTexture;
uniform float     u_exposure;
uniform float     u_near;
uniform float     u_far;
uniform float     u_screenWidth;
uniform float     u_aspectRatio;
uniform bool      u_useDOF;
uniform float     u_dofDistanceLinear;
uniform bool	  u_useVignette;

#ifdef HDR_ENABLED
const float GAMMA = 1.4;
const float GAMMA_RECIPROCAL = 1.0 / GAMMA;
#endif
const float BLUR_SIZE_HORIZONTAL = 1.0 / u_screenWidth;
const float BLUR_SIZE_VERTICAL = BLUR_SIZE_HORIZONTAL / u_aspectRatio;
const float SOBEL_SIZE_HORIZONTAL = 1.0 / 4000.0; //unused
const float SOBEL_SIZE_VERTICAL = SOBEL_SIZE_HORIZONTAL / u_aspectRatio; //unused

/*
this function takes current fragment depth (which is not linear)
and returns a linear value representation of it
do not divide by u_far when using this function as it has already done here
*/
float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // map back to NDC
    return (2.0 * u_near) / (u_far + u_near - z * (u_far - u_near)); // * u_far
}

/*
applying box blur for input fragment color
coordOffset - define the distance of the farthest neighbour to fetch,
thus defining dimension of the box as 1+coordOffset*2
depthClip - define whether or not a particular texel should be part of the source color,
i.e. neighbour texel which is close to camera obviously should not be included in blending
*/
void blur(inout vec3 fragColor, int coordOffset, float depthClip)
{
    vec3 blurredColor = vec3(0.0);
    //we need to keep track of how many neighbour texels have impact on the final color
    int texelFetches = 0;
    for (int x = -coordOffset; x <= coordOffset; x++)
    {
        for (int y = -coordOffset; y <= coordOffset; y++)
        {
            vec2 neighbourTexCoords = vec2(v_TexCoords.x + x * BLUR_SIZE_HORIZONTAL, v_TexCoords.y + y * BLUR_SIZE_VERTICAL);
            float neighbourTexelDepth = texture(u_frameDepthTexture, neighbourTexCoords).r;
            neighbourTexelDepth = linearizeDepth(neighbourTexelDepth);
            //also make sure that the most farthest texels (as environment/skybox etc.) do not get blurred
            if (neighbourTexelDepth > depthClip && neighbourTexelDepth < 0.999)
            {
                blurredColor += texture(u_frameTexture, neighbourTexCoords).rgb;
                ++texelFetches;
            }
        }
    }
    blurredColor /= texelFetches;
    fragColor = blurredColor;
}

/*
applying sobel post-process effect for input fragment color
original code: https://open.gl/framebuffers
currently unused
*/
void sobel(inout vec3 fragColor)
{
    vec4 top         = texture(u_frameTexture, vec2(v_TexCoords.x, v_TexCoords.y + SOBEL_SIZE_VERTICAL));
    vec4 bottom      = texture(u_frameTexture, vec2(v_TexCoords.x, v_TexCoords.y - SOBEL_SIZE_VERTICAL));
    vec4 left        = texture(u_frameTexture, vec2(v_TexCoords.x - SOBEL_SIZE_HORIZONTAL, v_TexCoords.y));
    vec4 right       = texture(u_frameTexture, vec2(v_TexCoords.x + SOBEL_SIZE_HORIZONTAL, v_TexCoords.y));
    vec4 topLeft     = texture(u_frameTexture, vec2(v_TexCoords.x - SOBEL_SIZE_HORIZONTAL, v_TexCoords.y + SOBEL_SIZE_VERTICAL));
    vec4 topRight    = texture(u_frameTexture, vec2(v_TexCoords.x + SOBEL_SIZE_HORIZONTAL, v_TexCoords.y + SOBEL_SIZE_VERTICAL));
    vec4 bottomLeft  = texture(u_frameTexture, vec2(v_TexCoords.x - SOBEL_SIZE_HORIZONTAL, v_TexCoords.y - SOBEL_SIZE_VERTICAL));
    vec4 bottomRight = texture(u_frameTexture, vec2(v_TexCoords.x + SOBEL_SIZE_HORIZONTAL, v_TexCoords.y - SOBEL_SIZE_VERTICAL));
    vec4 sobelX = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
    vec4 sobelY = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
    vec4 sobel = sqrt(sobelX * sobelX + sobelY * sobelY);
    fragColor = sobel.rgb;
}

void main()
{
    vec3 sampledColor = texture(u_frameTexture, v_TexCoords).rgb;
    if (u_useDOF)
    {
        float sampledDepth = texture(u_frameDepthTexture, v_TexCoords).r;
        sampledDepth = linearizeDepth(sampledDepth);
        //also make sure that the most farthest texels (as environment/skybox etc.) do not get blurred
        if (sampledDepth > u_dofDistanceLinear && sampledDepth < 0.999)
            blur(sampledColor, 1, u_dofDistanceLinear);
    }

    #ifdef HDR_ENABLED
    {
        vec3 toneCorrectedColor = vec3(1.0) - exp(-sampledColor * u_exposure);
        toneCorrectedColor = pow(toneCorrectedColor, vec3(GAMMA_RECIPROCAL));
        o_FragColor = vec4(toneCorrectedColor, 1.0);
    }
    #else
        o_FragColor = vec4(sampledColor, 1.0);
    #endif

    //finally apply vignette filter to a fragment's green and blue channels separately
	if(u_useVignette)
	{
		float vignetteSampledAlpha = texture(u_vignetteTexture, v_TexCoords).a;
		o_FragColor.gb *= 1.0 - vec2(pow(vignetteSampledAlpha, 5.0), pow(vignetteSampledAlpha, 4.0));
	}
}
