#version 450

out vec4 FragColor;

uniform bool cursorMode;

void main()
{
    if (cursorMode)
        FragColor = vec4(1.0, 0.5, 0.5, 0.5);
    else
        FragColor = vec4(1.0, 0.8, 0.0, 0.5);
}
