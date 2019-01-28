#version 450

layout (location = 0) in vec4  i_pos;
layout (location = 1) in vec3  i_normal;
layout (location = 2) in vec2  i_texCoords;

uniform mat4 u_projectionView;
uniform mat4 u_model;
uniform vec3 u_lightDir;
uniform int  u_type;

const int SKYSPHERE_TYPE_AMBIENT_LIGHTING = 0;
const int SKYSPHERE_TYPE_STARS = 1;

out vec2 v_TexCoords;
out vec3 v_Normal;

void main()
{
    float sunPositionAttenuation = mix(0.0, 1.0, clamp((u_lightDir.y + 0.05) * 8, 0.0, 1.0));
    float positionVerticalOffset = 0.0;
    if (u_type == SKYSPHERE_TYPE_AMBIENT_LIGHTING)
        positionVerticalOffset -= mix(0.0, 0.5, sunPositionAttenuation);
    else
        positionVerticalOffset -= 0.3;
    vec4 position = vec4((u_model * i_pos).xyz, 1.0);
    position.y += positionVerticalOffset;

    gl_Position = (u_projectionView * position).xyww;
    v_TexCoords = i_texCoords;
    v_Normal = mat3(u_model) * i_normal;
}
