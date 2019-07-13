#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;

uniform mat4      u_projectionView;
uniform float     u_mapDimensionReciprocal;
uniform sampler2D u_diffuseMixMap;
uniform float     u_underwaterSurfaceLevel;
uniform float     u_waterLevel;
//these two are used during scene reflection/refraction rendering routine
uniform bool      u_useClipDistanceReflection;
uniform bool      u_useClipDistanceRefraction;

const vec4  CLIP_PLANE_REFLECTION = vec4(0.0, 1.0, 0.0, u_waterLevel);
const vec4  CLIP_PLANE_REFRACTION = vec4(0.0, -1.0, 0.0, -u_waterLevel);
const float TERRAIN_TYPE_HEIGHT_DAMP_FACTOR = 1.75;

out vec3  v_FragPos;
out vec2  v_TexCoords;
//v_ShoreLandMix - define how much land or shore texture to use for a fragment
out float v_ShoreLandMix;
//v_ShoreUnderwaterMix - define how much underwater or shore texture to use for a fragment
out float v_ShoreUnderwaterMix;
out vec3  v_Normal;

void main()
{
    gl_Position = u_projectionView * i_pos;
    if (u_useClipDistanceReflection)
        gl_ClipDistance[0] = dot(CLIP_PLANE_REFLECTION, i_pos);
    if (u_useClipDistanceRefraction)
        gl_ClipDistance[0] = dot(CLIP_PLANE_REFRACTION, i_pos);

    v_FragPos = i_pos.xyz;
    v_TexCoords = i_texCoords;
    v_Normal = i_normal;

    float terrainSplattingRatio = texture(u_diffuseMixMap, i_pos.xz * u_mapDimensionReciprocal + 0.5).g;
    v_ShoreLandMix = i_pos.y * TERRAIN_TYPE_HEIGHT_DAMP_FACTOR + 1.5 - terrainSplattingRatio * 0.5;
    //for this its okay to clamp in a vertex shader stage
    v_ShoreUnderwaterMix = 1.0 - clamp((i_pos.y + u_underwaterSurfaceLevel) * 0.5 - 0.1, 0.0, 1.0);
}
