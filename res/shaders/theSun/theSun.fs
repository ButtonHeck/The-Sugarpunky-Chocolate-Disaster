#version 450

out vec4 o_FragColor;

uniform sampler2D u_texture;

in float v_PositionY;

void main()
{
    vec4 sampledDiffuse = texture(u_texture, gl_PointCoord);
    float colorMix = clamp(v_PositionY * 10, 0.0, 1.0);
    o_FragColor = mix(vec4(0.6, 0.05, 0.05, sampledDiffuse.a), sampledDiffuse, colorMix);
}
