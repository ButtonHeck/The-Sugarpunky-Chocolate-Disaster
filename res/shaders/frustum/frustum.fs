#version 450

out vec4 o_FragColor;

uniform bool u_isNear;
uniform bool u_isVolume;

void main()
{
    if (u_isVolume)
    {
        if (u_isNear)
            o_FragColor = vec4(0.1, 0.1, 1.0, 1.0);
        else
            o_FragColor = vec4(0.0, 0.0, 0.5, 1.0);
    }
    else
    {
        if (u_isNear)
            o_FragColor = vec4(0.1, 1.0, 0.1, 1.0);
        else
            o_FragColor = vec4(0.0, 0.5, 0.0, 1.0);
    }
}
