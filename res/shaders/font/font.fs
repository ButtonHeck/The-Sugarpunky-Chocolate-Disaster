#version 450

in vec2  v_TexCoords;
out vec4 o_FragColor;

uniform sampler2D u_fontTextureAtlas;

void main()
{
    float opacityMask = texture( u_fontTextureAtlas, v_TexCoords ).a;
    o_FragColor = vec4( 0.0, 0.0, 0.0, opacityMask );
}
