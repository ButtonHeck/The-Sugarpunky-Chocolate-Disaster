#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;
layout (location = 3) in vec3 i_tangent;
layout (location = 4) in vec3 i_bitangent;

/*
is using for land/hills texture splatting "randomization"
the higher the value - the higher land splats with hills
keep it in range[0;1]
*/
const float TERRAIN_TYPE_HEIGHT_DAMP_FACTOR = 0.75;
const float SPECULAR_SHININESS = 64.0;

uniform mat4        u_projectionView;
uniform vec3        u_lightDir;
//u_mapDimensionReciprocal - used for different tiling techniques
uniform float       u_mapDimensionReciprocal;
//u_diffuseMixMap - is using for land/hills texture splatting "randomization"
uniform sampler2D   u_diffuseMixMap;
uniform vec3        u_viewPosition;

out vec3  v_FragPos;
out vec3  v_Normal;
out vec2  v_TexCoords;
//v_TerrainTypeMix - define how much land or hill texture to use for a fragment
out float v_TerrainTypeMix;
out float v_SpecularComponent;

/*
TNB instead of TBN because in this shader normalMap blue component is treated as Y, not Z,
and green component represents Z, not Y (it also should be reflected, but there is no principal
difference). The whole idea was to represent normal map texture not as a plane with mainly positive Z normal,
but rather one that is rotated -90 degrees around X axis
(thus why G must be reflected - what was +Y turns out to be -Z)
*/
out mat3  v_TNB;

void main()
{
    gl_Position = u_projectionView * vec4( i_pos, 1.0 );

    v_FragPos = i_pos;
    v_Normal = i_normal;
    v_TexCoords = i_texCoords;
    v_TNB = mat3( i_tangent, i_normal, i_bitangent );

    float terrainSplattingRatio = texture( u_diffuseMixMap, i_pos.xz * u_mapDimensionReciprocal + 0.5 ).g;
    v_TerrainTypeMix = i_pos.y * ( 1.0 - terrainSplattingRatio * TERRAIN_TYPE_HEIGHT_DAMP_FACTOR );

    //specular component
    vec3 lightDirReflected = reflect( -u_lightDir, i_normal );
    vec3 viewDirection = normalize( u_viewPosition - i_pos );
    v_SpecularComponent = pow( max( dot( lightDirReflected, viewDirection ), 0.0 ), SPECULAR_SHININESS );
}
