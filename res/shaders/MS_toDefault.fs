#version 450

out vec4 o_FragColor;

uniform sampler2D u_frameTexture;

in vec2 v_TexCoords;

void main()
{
    vec3 color = texture(u_frameTexture, v_TexCoords).rgb;
    o_FragColor = vec4(color, 1.0);
}
