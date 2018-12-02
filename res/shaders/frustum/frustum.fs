#version 450

out vec4 o_FragColor;

uniform bool u_isNear;
uniform bool u_isExpectedVolume;
uniform bool u_isActualVolume;
uniform bool u_isLightSource;

void main()
{
    if (u_isLightSource)
    {
        if (u_isNear)
            o_FragColor = vec4(1.0, 0.5, 0.0, 1.0);
        else
            o_FragColor = vec4(0.5, 0.25, 0.0, 1.0);
    }
    else if (u_isActualVolume)
    {
        if (u_isNear)
            o_FragColor = vec4(1.0, 0.1, 0.1, 1.0);
        else
            o_FragColor = vec4(0.5, 0.0, 0.0, 1.0);
    }
    else if (u_isExpectedVolume)
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
