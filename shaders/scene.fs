#version 450

in vec2 TexCoords;
in float PosHeight;

uniform float HeightOffset;
uniform sampler2D grassTexture;
uniform sampler2D hillTexture;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(grassTexture, TexCoords), texture(hillTexture, TexCoords), min(max(0.0, PosHeight + HeightOffset), 1.0));
    //FragColor = texture(hillTexture, TexCoords);
}
