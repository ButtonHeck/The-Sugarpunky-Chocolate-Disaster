#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;

uniform mat4      u_projectionView;
uniform float     u_mapDimension;
uniform sampler2D u_diffuse_mix_map;
uniform float     U_UNDERWATER_TILE_YPOS;

out vec3  v_FragPos;
out vec2  v_TexCoords;
out float v_TerrainTypeMix;
out vec3  v_Normal;
out float v_AlphaBlend;

const float TERRAIN_TYPE_TRANSITION_RATIO = 1.75;

void main()
{
    gl_Position = u_projectionView * i_pos;

    v_FragPos = i_pos.xyz;
    v_TexCoords = i_texCoords;
    v_Normal = i_normal;

    float TerrainSplattingRatio = texture(u_diffuse_mix_map, i_pos.xz * u_mapDimension + 0.5).g;
    v_TerrainTypeMix = i_pos.y * TERRAIN_TYPE_TRANSITION_RATIO + 1.5 - TerrainSplattingRatio * 0.5;
    v_AlphaBlend = (i_pos.y + U_UNDERWATER_TILE_YPOS) * 0.5 - TerrainSplattingRatio * 0.25;
}
