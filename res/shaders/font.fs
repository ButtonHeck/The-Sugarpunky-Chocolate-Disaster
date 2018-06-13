#version 450

in vec2  v_TexCoords;
out vec4 o_FragColor;

uniform sampler2D u_text;
uniform vec3      u_textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, v_TexCoords).r);
    o_FragColor = vec4(u_textColor, 1.0) * sampled;
}
