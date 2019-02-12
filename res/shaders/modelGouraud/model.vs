#version 450

layout (location = 0) in vec4  i_pos;
layout (location = 1) in vec3  i_normal;
layout (location = 2) in vec2  i_texCoords;
layout (location = 3) in vec3  i_tangent;
layout (location = 4) in vec3  i_bitangent;
layout (location = 5) in mat4  i_model;
/*
all models have one diffuse texture and may have one specular,
this variable contain a pair of indices in texture array of each texture type
*/
layout (location = 9) in uvec2 i_texIndices;

const float MAX_ANIMATION_DISTANCE = 30.0;
const float SPECULAR_SHININESS = 4.0;

uniform mat4  u_projectionView;
uniform vec3  u_lightDir;
uniform vec3  u_viewPosition;
uniform float u_ambientDay;

out vec2        v_TexCoords;
out float       v_DiffuseComponent;
out float       v_SpecularComponent;
out float       v_SunPositionAttenuation;
out float       v_NormalY;
out vec3        v_FragPos;
flat out uvec2  v_TexIndices;

@include modelGrassAnimationAndBlending.ivs

void main()
{
    vec4 worldPosition = i_model * i_pos;
    v_FragPos = vec3(worldPosition);

    float distanceToObject = distance(u_viewPosition, v_FragPos);
    //initially set this to 4.0 (watch comments in modelGrassAnimationAndBlending file)
    v_AlphaValue = 4.0;

    if (distanceToObject < MAX_ANIMATION_DISTANCE)
        ext_animateAndBlend(worldPosition);

    gl_Position = u_projectionView * worldPosition;
    v_TexCoords = i_texCoords;
    v_TexIndices = i_texIndices;

    vec3 normal = normalize(vec3(i_model * vec4(i_normal, 0)));
    v_NormalY = normal.y;

    v_SunPositionAttenuation = mix(0.0, 1.0, clamp(u_lightDir.y * 3, 0.0, 1.0));
    vec3 shadingNormal = normal;

    //diffuse
    v_DiffuseComponent = max(dot(shadingNormal, u_lightDir), 0.0) * v_SunPositionAttenuation * (1.0 - u_ambientDay);
    if (u_isGrass)
    {
        /*
        make grass diffuse lighting look more natural by calculating diffuse component for its reversed normal
        because there are no backfaces for the grass. Then make it no less than defined minimum (calculated empirically)
        in cases a leaf is almost vertical (horizontally oriented normal) and the sun is at zenith (vertical light direction),
        otherwise grass coloring would look unnatural too.
        */
        float diffuseComponentNegative = max(dot(-shadingNormal, u_lightDir), 0.0) * v_SunPositionAttenuation * (1.0 - u_ambientDay);
        v_DiffuseComponent = max(v_DiffuseComponent, diffuseComponentNegative);
        v_DiffuseComponent = mix(v_DiffuseComponent, max(v_DiffuseComponent, 0.33), v_SunPositionAttenuation);
    }

    //specular
    vec3 lightDirectionReflected = reflect(-u_lightDir, normal);
    vec3 viewDirection = normalize(u_viewPosition - v_FragPos);
    v_SpecularComponent = pow(max(dot(lightDirectionReflected, viewDirection), 0.0), SPECULAR_SHININESS) * v_SunPositionAttenuation;
}
