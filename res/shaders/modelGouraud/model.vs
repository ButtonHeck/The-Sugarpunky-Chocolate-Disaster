#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texCoords;
layout (location = 3) in vec3 i_tangent;
layout (location = 4) in vec3 i_bitangent;
layout (location = 5) in mat4 i_model;

uniform mat4  u_projectionView;
uniform vec3  u_lightDir;
uniform vec3  u_viewPosition;

out vec2  v_TexCoords;
out float v_DiffuseComponent;
out float v_SpecularComponent;
out float v_SunPositionAttenuation;
out float v_NormalY;
out vec3  v_FragPos;

@include modelGrassAnimationAndBlending.ivs

void main()
{
    vec4 ModelWorldPosition = i_model * i_pos;
    v_FragPos = vec3(ModelWorldPosition);

    float distanceToObject = distance(u_viewPosition, v_FragPos);
    float normalDistributionImitation = 1.0;
    v_AlphaValue = 4.0;

    if (distanceToObject < 30.0)
        ext_animateAndBlend(ModelWorldPosition, normalDistributionImitation);

    gl_Position = u_projectionView * ModelWorldPosition;
    v_TexCoords = i_texCoords;

    vec3 normal = normalize(vec3(i_model * vec4(i_normal, 0)));
    v_NormalY = normal.y;
    v_NormalY *= normalDistributionImitation; //as far as we render no grass this stays 1.0

    v_SunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 5, 0.0, 1.0));
    vec3 shadingNormal = normal;
    if (u_isGrass)
        shadingNormal.y *= sign(shadingNormal.y) * mix(1.0, u_lightDir.y, v_SunPositionAttenuation); //intentionally left unnormalized

    //diffuse
    v_DiffuseComponent = max(dot(shadingNormal, u_lightDir), 0.0) * v_SunPositionAttenuation;

    //specular
    vec3 Reflect = reflect(-u_lightDir, normal);
    vec3 ViewDir = normalize(u_viewPosition - v_FragPos);
    v_SpecularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 4.0) * 0.75 * v_SunPositionAttenuation;
}
