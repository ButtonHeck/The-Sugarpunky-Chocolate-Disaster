#version 450

in vec2  v_TexCoords;
out vec4 o_FragColor;

uniform sampler2D u_fontTexture;

void main()
{
    float sampledAlpha = texture(u_fontTexture, v_TexCoords).a;
    o_FragColor = vec4(0.0, 0.0, 0.0, sampledAlpha);
}
