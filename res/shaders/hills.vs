#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;

uniform mat4        u_projectionView;
uniform vec3        u_lightDir;
uniform int         u_mapDimension;
uniform sampler2D   u_normal_map;
uniform vec3        u_viewPosition;
uniform vec4        u_frustumPlanes[4]; //back and front planes sucks!

out vec2  v_TexCoords;
out float v_PosHeight;
out float v_DiffuseComponentHill;
out float v_DiffuseComponentFlat;
out float v_TextureFlatMixRatio;
out float v_TextureHillMixRatio;
out float v_SpecularComponent;
out int   v_visible;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);
const float TRANSITION_RATIO_MULTIPLIER = 1.5;
const float TILE_RADIUS = sqrt(2);

//frustum culling function itself
int cullThisSonOfaBitch()
{
    for (int i = 0; i < 4; i++)
    {
        if (dot(u_frustumPlanes[i].xyz, i_pos) <= -u_frustumPlanes[i].w - TILE_RADIUS)
          return 0;
    }
  return 1;
}

void main()
{
    v_visible = cullThisSonOfaBitch();
    gl_Position = u_projectionView * vec4(i_pos, 1.0);
    v_TexCoords = i_texCoords;
    v_PosHeight = i_pos.y / TRANSITION_RATIO_MULTIPLIER;

    //"normal" vector fetched from the global normal map (same as flat terrain normal), it is used for randomization of some variables
    vec3 FlatNormal = texture(u_normal_map, vec2(i_pos.x / u_mapDimension + 0.5, i_pos.z / u_mapDimension + 0.5)).rgb;
    v_TextureFlatMixRatio = FlatNormal.r;
    v_TextureHillMixRatio = FlatNormal.r * TRANSITION_RATIO_MULTIPLIER;

    //calculate how much each normal (flat or hill) would define the actual shading normal vector
    float TransitionRatio = clamp(0.0 + i_pos.y * TRANSITION_RATIO_MULTIPLIER, 0.0, 1.0);
    //calculate the actual normal vector which then is used for shading
    vec3 ShadingNormal = normalize(TransitionRatio * i_normal + (1.0 - TransitionRatio) * FlatNormal);

    //diffuse component
    v_DiffuseComponentHill = max(dot(ShadingNormal, u_lightDir), 0.0);
    v_DiffuseComponentFlat = max(dot(normalize(NORMAL + FlatNormal), u_lightDir), 0.0);

    //specular component
    vec3 Reflect = reflect(-u_lightDir, ShadingNormal);
    vec3 ViewDir = normalize(u_viewPosition - i_pos);
    v_SpecularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 64.0);
}
