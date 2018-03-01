#version 450

in vec2 TexCoords;
in float PosHeight;

uniform sampler2D grassTexture;
uniform sampler2D hillTexture;
uniform sampler2D waterTexture;
uniform int waterRendering;

out vec4 FragColor;

void main()
{
    if (waterRendering == 0)
        FragColor = mix(texture(grassTexture, TexCoords), texture(hillTexture, TexCoords), min(max(0.0, PosHeight), 1.0));
    else
        FragColor = texture(waterTexture, TexCoords);
    //FragColor = texture(hillTexture, TexCoords);
}
