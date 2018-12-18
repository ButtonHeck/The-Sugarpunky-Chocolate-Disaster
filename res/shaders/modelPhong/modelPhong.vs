#version 450

layout (location = 0) in vec4  i_pos;
layout (location = 1) in vec3  i_normal;
layout (location = 2) in vec2  i_texCoords;
layout (location = 3) in vec3  i_tangent;
layout (location = 4) in vec3  i_bitangent;
layout (location = 5) in mat4  i_model;
layout (location = 9) in uvec2 i_texIndices;

uniform mat4  u_projectionView;
uniform vec3  u_viewPosition;

out vec2        v_TexCoords;
out vec3        v_Normal;
out vec3        v_FragPos;
flat out uvec2  v_TexIndices;

@include modelGrassAnimationAndBlending.ivs

void main()
{
    vec4 ModelWorldPosition = i_model * i_pos;
    v_FragPos = vec3(ModelWorldPosition);

    float distanceToObject = distance(u_viewPosition, v_FragPos);
    float normalDistributionImitation = 1.0;
    v_AlphaValue = 4.0;

    if (distanceToObject < 30.0)
        ext_animateAndBlend(ModelWorldPosition, normalDistributionImitation);

    gl_Position = u_projectionView * ModelWorldPosition;
    v_TexCoords = i_texCoords;
    v_TexIndices = i_texIndices;
    v_Normal = vec3(i_model * vec4(i_normal, 0));
    v_Normal.y *= normalDistributionImitation; //as far as we render no grass this stays 1.0
}
