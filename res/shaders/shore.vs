#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;

uniform mat4      u_projectionView;
uniform vec3      u_lightDir;
uniform sampler2D u_normal_map;
uniform int       u_mapDimension;

out vec2  v_TexCoords;
out vec3  v_FragPos;
out float v_PosHeight;
out float v_TextureMixRatio;
out float v_DiffuseComponent;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);
const float POSITION_HEIGHT_MULTIPLIER = 2.1;

void main()
{
    gl_Position = u_projectionView * vec4(i_pos, 1.0);
    v_FragPos = i_pos;
    v_TexCoords = i_texCoords;
    v_PosHeight = i_pos.y * POSITION_HEIGHT_MULTIPLIER;

    vec3 FlatNormal = texture(u_normal_map, vec2(v_FragPos.x / u_mapDimension + 0.5, v_FragPos.z / u_mapDimension + 0.5)).rgb;
    float TransitionRatio = clamp(1.0 + v_PosHeight, 0.0, 1.0);
    vec3 ShadingNormal = normalize((1.0 - TransitionRatio) * i_normal + TransitionRatio * (NORMAL + FlatNormal));
    v_TextureMixRatio = FlatNormal.r;

    //diffuse
    v_DiffuseComponent = max(dot(ShadingNormal, u_lightDir), 0.0);
}
