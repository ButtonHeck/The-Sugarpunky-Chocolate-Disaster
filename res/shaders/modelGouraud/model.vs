#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texCoords;
layout (location = 3) in vec3 i_tangent;
layout (location = 4) in vec3 i_bitangent;
layout (location = 5) in mat4 i_model;

uniform mat4  u_projectionView;
uniform mat4  u_lightSpaceMatrix;
uniform vec3  u_lightDir;
uniform bool  u_isGrass;
uniform float u_grassPosDistrubution;
uniform vec3  u_viewPosition;

const vec3 GRASS_WAVE_XYZ = vec3(0.007, 0.0023, 0.0035);

out vec2  v_TexCoords;
out float v_DiffuseComponent;
out float v_SpecularComponent;
out vec3  v_ProjectedCoords;
out float v_AlphaValue;
out float v_SunPositionAttenuation;
out float v_NormalY;

void main()
{
    vec4 ModelWorldPosition = i_model * i_pos;
    vec3 FragPos = vec3(ModelWorldPosition);

    float distanceToObject = distance(u_viewPosition, FragPos);
    float normalDistributionImitation = 1.0;
    v_AlphaValue = 1.0;

    if (distanceToObject < 30.0)
    {
        if (u_isGrass)
        {
            if (i_pos.y > 0.38)
            {
                float fractionOfWorldX = fract(ModelWorldPosition.x); //coarse phase offset tweak
                float fractionOfWorldZ = fract(ModelWorldPosition.z); //fine phase offset tweak
                float influence = sin(u_grassPosDistrubution * max(fractionOfWorldX, 0.75) + fractionOfWorldZ) * 0.5 + 0.5;
                ModelWorldPosition.xyz += (mix(GRASS_WAVE_XYZ,
                                               GRASS_WAVE_XYZ + i_normal * 0.005,
                                               dot(normalize(GRASS_WAVE_XYZ), i_normal)))
                                         * influence;
                normalDistributionImitation = 0.66 * influence + 0.66;
            }
            v_AlphaValue = ModelWorldPosition.y * 32;
        }
        else
        {
            v_AlphaValue = ModelWorldPosition.y * 48;
        }
    }

    gl_Position = u_projectionView * ModelWorldPosition;
    v_TexCoords = i_texCoords;

    vec3 normal = normalize(vec3(i_model * vec4(i_normal, 0)));
    v_NormalY = normal.y;
    v_NormalY *= normalDistributionImitation; //as far as we render no grass this stays 1.0
    vec4 fragPosLightSpace = u_lightSpaceMatrix * ModelWorldPosition;
    v_ProjectedCoords = fragPosLightSpace.xyz * 0.5 + 0.5; //transform from [-1;1] to [0;1]

    //diffuse
    vec3 shadingNormal = normal;
    if (u_isGrass)
        shadingNormal.y *= sign(shadingNormal.y) * u_lightDir.y; //intentionally left unnormalized

    v_SunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 5, 0.0, 1.0));
    v_DiffuseComponent = max(dot(shadingNormal, u_lightDir), 0.0) * v_SunPositionAttenuation;

    //specular
    vec3 Reflect = reflect(-u_lightDir, normal);
    vec3 ViewDir = normalize(u_viewPosition - FragPos);
    v_SpecularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 4.0) * 0.75 * v_SunPositionAttenuation;
}
