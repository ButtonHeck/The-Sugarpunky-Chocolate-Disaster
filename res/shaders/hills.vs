#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;

uniform mat4        u_projectionView;
uniform vec3        u_lightDir;
uniform float       u_mapDimension;
uniform sampler2D   u_diffuse_mix_map;
uniform vec3        u_viewPosition;
uniform mat4        u_lightSpaceMatrix;

out vec3  v_FragPos;
out vec3  v_Normal;
out vec2  v_TexCoords;
out float v_PosHeight;
out float v_TextureMixRatio;
out float v_SpecularComponent;
out vec3  v_ProjectedCoords;

const vec3 NORMAL = vec3(0.0, 1.0, 0.0);
const float DEPTH_BIAS = 0.01;

void main()
{
    gl_Position = u_projectionView * vec4(i_pos, 1.0);

    v_FragPos = i_pos;
    v_Normal = i_normal;
    v_TexCoords = i_texCoords;
    vec4 fragPosLightSpace = u_lightSpaceMatrix * vec4(i_pos, 1.0);
    v_ProjectedCoords = fragPosLightSpace.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]

    vec3 DiffuseTextureMix = texture(u_diffuse_mix_map, i_pos.xz * u_mapDimension + 0.5).rgb;
    v_PosHeight = i_pos.y * (1.0 - DiffuseTextureMix.g * 0.75);
    v_TextureMixRatio = DiffuseTextureMix.r;

    //calculate how much each normal (flat or hill) would define the actual shading normal vector
    float TransitionRatio = clamp(i_pos.y, 0.0, 1.0);

    //specular component
    vec3 Reflect = reflect(-u_lightDir, i_normal);
    vec3 ViewDir = normalize(u_viewPosition - i_pos);
    v_SpecularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 64.0);
}
