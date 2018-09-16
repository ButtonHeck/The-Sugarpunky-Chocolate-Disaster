#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texCoords;
layout (location = 3) in vec3 i_tangent;
layout (location = 4) in vec3 i_bitangent;
layout (location = 5) in mat4 i_model;

uniform mat4  u_projectionView;
uniform mat4  u_lightSpaceMatrix;
uniform bool  u_isGrass;
uniform float u_grassPosDistribution;
uniform float u_grassPosDistrubutionInfluence;

const vec3 GRASS_WAVE_XYZ = vec3(0.007, 0.0023, 0.0035);

out vec2  v_TexCoords;
out vec3  v_Normal;
out vec3  v_ProjectedCoords;
out float v_FlatBlend;
out vec3  v_FragPos;

void main()
{
    vec4 ModelWorldPosition = i_model * i_pos;
    if (u_isGrass)
    {
        if (i_pos.y > 0.38)
        {
            float influence = sin(u_grassPosDistrubutionInfluence * fract(max(ModelWorldPosition.x, 0.1)) * 2) * 0.5 + 0.5;
            float distribution = cos(u_grassPosDistribution * fract(max(ModelWorldPosition.z, 0.1)) * 2) * 2.0;
            ModelWorldPosition.xyz += (mix(GRASS_WAVE_XYZ, GRASS_WAVE_XYZ + i_normal * 0.005, dot(normalize(GRASS_WAVE_XYZ), i_normal)))
                                    * distribution * influence * clamp(i_pos.y, 0.0, 1.0);
        }
        v_FlatBlend = ModelWorldPosition.y * 32;
    }
    else
    {
        v_FlatBlend = ModelWorldPosition.y * 48;
    }

    gl_Position = u_projectionView * ModelWorldPosition;
    v_TexCoords = i_texCoords;
    v_FragPos = vec3(ModelWorldPosition);
    v_Normal = vec3(i_model * vec4(i_normal, 0));
    vec4 fragPosLightSpace = u_lightSpaceMatrix * ModelWorldPosition;
    v_ProjectedCoords = fragPosLightSpace.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]
}
