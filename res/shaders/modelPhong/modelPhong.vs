#version 450

layout (location = 0) in vec4  i_pos;
layout (location = 1) in vec3  i_normal;
layout (location = 2) in vec2  i_texCoords;
layout (location = 3) in vec3  i_tangent;
layout (location = 4) in vec3  i_bitangent;
layout (location = 5) in mat4  i_model;
/*
all models have one diffuse texture and may have one specular,
this variable contain a pair of indices in texture array of each texture type
*/
layout (location = 9) in uvec2 i_texIndices;

const float MAX_ANIMATION_DISTANCE = 30.0;

uniform mat4  u_projectionView;
uniform vec3  u_viewPosition;

out vec2        v_TexCoords;
out vec3        v_Normal;
out vec3        v_FragPos;
flat out uvec2  v_TexIndices;

@include modelGrassAnimation.ivs

void main()
{
    vec4 worldPosition = i_model * i_pos;
    v_FragPos = vec3(worldPosition);

    float distanceToObject = distance(u_viewPosition, v_FragPos);

    if (distanceToObject < MAX_ANIMATION_DISTANCE)
        ext_animate(worldPosition);

    gl_Position = u_projectionView * worldPosition;
    v_TexCoords = i_texCoords;
    v_TexIndices = i_texIndices;
    v_Normal = vec3(i_model * vec4(i_normal, 0));
}
