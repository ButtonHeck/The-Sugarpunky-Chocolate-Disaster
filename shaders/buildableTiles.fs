#version 450

out vec4 o_FragColor;

uniform bool u_cursorMode;

void main()
{
    if (u_cursorMode)
        o_FragColor = vec4(1.0, 0.3, 0.5, 0.33);
    else
        o_FragColor = vec4(1.0, 0.8, 0.0, 0.33);
}
