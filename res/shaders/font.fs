#version 450

in vec2  v_TexCoords;
out vec4 o_FragColor;

uniform sampler2D u_glyph;
uniform vec4      u_glyphColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_glyph, v_TexCoords).r);
    o_FragColor = u_glyphColor * sampled;
}
