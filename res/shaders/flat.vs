#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 3) in mat4 i_instanceModel;

uniform mat4        u_projectionView;
uniform vec3        u_lightDir;
uniform mat4        u_lightSpaceMatrix;
uniform sampler2D   u_shadowMap;

out vec2 v_TexCoords;
out vec3 v_FragPos;
out vec3 v_LightDir;
out vec3 v_ProjectedCoords;

void main()
{
    gl_Position = u_projectionView * i_instanceModel * i_pos;
    v_FragPos = vec3(i_instanceModel * i_pos);
    v_TexCoords = i_texCoords;
    v_LightDir = u_lightDir;
    vec4 fragPosLightSpace = u_lightSpaceMatrix * vec4(v_FragPos, 1.0);
    v_ProjectedCoords = fragPosLightSpace.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]
}
