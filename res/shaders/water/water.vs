#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;

uniform mat4 u_projectionView;
uniform mat4 u_lightSpaceMatrix;

out vec3  v_FragPos;
out vec3  v_Normal;
out vec3  v_ProjectedCoords;

void main()
{
    gl_Position = u_projectionView * vec4(i_pos, 1.0);
    v_FragPos = i_pos;
    v_Normal = i_normal;
    vec4 fragPosLightSpace = u_lightSpaceMatrix * vec4(v_FragPos, 1.0);
    v_ProjectedCoords = fragPosLightSpace.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]
}
