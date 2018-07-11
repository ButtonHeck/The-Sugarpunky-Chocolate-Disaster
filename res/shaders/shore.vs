#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;

uniform mat4      u_projectionView;
uniform vec3      u_lightDir;
uniform sampler2D u_normal_map;
uniform float     u_mapDimension;
uniform mat4      u_lightSpaceMatrix;

out vec2  v_TexCoords;
out float v_PosHeight;
out float v_PositionDiffuseComponent;
out float v_TextureMixRatio;
out float v_DiffuseComponent;
out vec3  v_Normal;
out vec3  v_ProjectedCoords;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);
const float POSITION_HEIGHT_MULTIPLIER = 2.1;

void main()
{
    gl_Position = u_projectionView * i_pos;
    v_TexCoords = i_texCoords;
    v_PosHeight = i_pos.y * POSITION_HEIGHT_MULTIPLIER;
    vec4 fragPosLightSpace = u_lightSpaceMatrix * i_pos;
    v_ProjectedCoords = fragPosLightSpace.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]

    vec3 FlatNormal = texture(u_normal_map, i_pos.xz * u_mapDimension + 0.5).rgb;
    float TransitionRatio = clamp(1.0 + v_PosHeight, 0.0, 1.0);
    vec3 ShadingNormal = normalize(mix(i_normal, NORMAL + FlatNormal, TransitionRatio));
    v_Normal = ShadingNormal;
    v_TextureMixRatio = FlatNormal.r;

    //diffuse
    v_PositionDiffuseComponent = 1.0 + i_pos.y * 0.4;
    v_DiffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0);
}
