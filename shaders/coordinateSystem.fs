#version 450

in vec3 v_fColor;

out vec4 o_FragColor;

void main()
{
    o_FragColor = vec4(v_fColor, 1.0);
}
