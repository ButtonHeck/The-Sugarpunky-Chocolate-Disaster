#version 450

out vec4 FragColor;

uniform sampler2D grassTexture;

in vec2 TexCoords;

void main()
{
    vec4 color = texture(grassTexture, TexCoords);
    if (color.a < 0.1)
        discard;
    FragColor = color;
}
