#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texCoords;
layout (location = 3) in vec3 i_tangent;
layout (location = 4) in vec3 i_bitangent;
layout (location = 5) in mat4 i_model;

uniform mat4  u_projectionView;
uniform vec3  u_viewPosition;
uniform vec3  u_lightDir;
uniform mat4  u_lightSpaceMatrix;
uniform bool  u_isGrass;
uniform float u_grassPosDistribution;
uniform float u_grassPosDistrubutionInfluence;

const vec3 GRASS_WAVE_XYZ = vec3(0.0055, 0.00195, 0.0025);

out vec2  v_TexCoords;
out vec3  v_Normal;
out float v_DiffuseComponent;
out float v_SpecularComponent;
out vec3  v_ProjectedCoords;

void main()
{
    vec4 ModelWorldPosition = i_model * i_pos;
    if (u_isGrass && i_pos.y > 0.45)
    {
        float influence = sin(u_grassPosDistrubutionInfluence + fract(i_pos.y) * 41);
        float distribution = cos(u_grassPosDistribution + fract(i_pos.x) * 29);
        ModelWorldPosition.xyz += (mix(GRASS_WAVE_XYZ, GRASS_WAVE_XYZ + i_normal * 0.005, dot(normalize(GRASS_WAVE_XYZ), i_normal)))
            * distribution * influence * clamp(i_pos.y, 0.0, 1.0);
    }
    gl_Position = u_projectionView * ModelWorldPosition;
    v_TexCoords = i_texCoords;
    vec3 FragPos = vec3(ModelWorldPosition);
    v_Normal = normalize(vec3(i_model * vec4(i_normal, 0)));
    vec4 fragPosLightSpace = u_lightSpaceMatrix * ModelWorldPosition;
    v_ProjectedCoords = fragPosLightSpace.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]

    //diffuse
    v_DiffuseComponent = max(dot(v_Normal, u_lightDir), 0.0);

    //specular
    vec3 Reflect = reflect(-u_lightDir, v_Normal);
    vec3 ViewDir = normalize(u_viewPosition - FragPos);
    v_SpecularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 8.0) * 0.33;
}
