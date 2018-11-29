#version 450

out vec4 o_FragColor;

uniform bool u_isNear;

void main()
{
    if (u_isNear)
        o_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    else
        o_FragColor = vec4(0.0, 0.4, 0.0, 1.0);
}
