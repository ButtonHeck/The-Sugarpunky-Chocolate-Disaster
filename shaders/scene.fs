#version 450

in vec2 TexCoords;
in float PosHeight;

uniform sampler2D grassTexture;
uniform sampler2D hillTexture;
uniform sampler2D waterTexture;
uniform sampler2D sandTexture;
uniform int surfaceTextureEnum;
uniform float waterLevel;

out vec4 FragColor;

void main()
{
    if (surfaceTextureEnum == 2)
    {
        FragColor = mix(texture(grassTexture, TexCoords), texture(hillTexture, TexCoords), min(max(0.0, PosHeight/2), 1.0));

    }
    else if (surfaceTextureEnum == 0)
    {
        FragColor = mix(texture(sandTexture, TexCoords), texture(grassTexture, TexCoords), PosHeight-waterLevel+(1+waterLevel));
    }
    else
        FragColor = texture(waterTexture, TexCoords);
}
