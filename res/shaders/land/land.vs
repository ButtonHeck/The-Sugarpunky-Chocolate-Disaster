#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec4 i_instanceTranslation;

uniform mat4 u_projectionView;

out vec2 v_TexCoords;
out vec3 v_FragPos;

void main()
{
    vec4 worldPosition = i_pos + i_instanceTranslation;
    gl_Position = u_projectionView * worldPosition;
    v_FragPos = vec3(worldPosition);
    v_TexCoords = i_texCoords;
}
