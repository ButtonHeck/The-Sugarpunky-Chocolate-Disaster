#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 3) in mat4 i_instanceModel;

uniform mat4 u_projectionView;
uniform vec3 u_lightDir;

out vec2 v_TexCoords;
out vec3 v_FragPos;
out vec3 v_LightDir;

void main()
{
    gl_Position = u_projectionView * i_instanceModel * vec4(i_pos, 1.0);
    v_FragPos = vec3(i_instanceModel * vec4(i_pos, 1.0));
    v_TexCoords = i_texCoords;
    v_LightDir = u_lightDir;
}
