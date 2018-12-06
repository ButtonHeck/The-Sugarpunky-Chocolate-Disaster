#version 450

out vec4 o_FragColor;

uniform int u_colorIndex;

const vec4 COLORS[12] =
{
//light source colors
    vec4(1.0, 0.5, 0.0, 1.0),       //near      0
    vec4(0.5, 0.25, 0.0, 1.0),      //middle    1
    vec4(0.25, 0.125, 0.0, 1.0),    //far       2
//frustum borders
    vec4(0.1, 1.0, 0.1, 1.0),       //near      3
    vec4(0.0, 0.5, 0.0, 1.0),       //middle    4
    vec4(0.0, 0.25, 0.0, 1.0),      //far       5
//expected volumes
    vec4(0.1, 0.1, 1.0, 1.0),       //near      6
    vec4(0.0, 0.0, 0.5, 1.0),       //middle    7
    vec4(0.0, 0.0, 0.25, 1.0),      //far       8
//actual volumes
    vec4(1.0, 0.1, 0.1, 1.0),       //near      9
    vec4(0.5, 0.0, 0.0, 1.0),       //middle    10
    vec4(0.25, 0.0, 0.0, 1.0)       //far       11
};

void main()
{
    o_FragColor = COLORS[u_colorIndex];
}
