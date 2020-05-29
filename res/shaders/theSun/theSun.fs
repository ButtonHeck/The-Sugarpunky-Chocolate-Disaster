#version 450

out vec4 o_FragColor;

uniform sampler2D u_texture;

//v_PositionY - used for interpolate from pink color to texture during dusk/dawn
in float v_PositionY;

const vec3 DUSK_DAWN_COLOR = vec3( 0.6, 0.05, 0.05 );

void main()
{
    vec4 sampledDiffuse = texture( u_texture, gl_PointCoord );
    float colorMix = clamp( v_PositionY * 10, 0.0, 1.0 );
    o_FragColor = mix( vec4( DUSK_DAWN_COLOR, sampledDiffuse.a ), sampledDiffuse, colorMix );
}
