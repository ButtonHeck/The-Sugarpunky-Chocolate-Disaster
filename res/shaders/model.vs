#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texCoords;
layout (location = 3) in vec3 i_tangent;
layout (location = 4) in vec3 i_bitangent;
layout (location = 5) in mat4 i_model;

uniform mat4 u_projectionView;
uniform vec3 u_viewPosition;
uniform vec3 u_lightDir;
uniform mat4 u_lightSpaceMatrix;

out vec2  v_TexCoords;
out vec3  v_Normal;
out float v_DiffuseComponent;
out float v_SpecularComponent;
out vec4  v_FragPosLightSpace;

void main()
{
    gl_Position = u_projectionView * i_model * vec4(i_pos, 1.0);
    v_TexCoords = i_texCoords;
    vec3 FragPos = vec3(i_model * vec4(i_pos, 1.0));
    v_Normal = normalize(mat3(transpose(inverse(i_model))) * i_normal);
    v_FragPosLightSpace = u_lightSpaceMatrix * i_model * vec4(i_pos, 1.0);

    //diffuse
    v_DiffuseComponent = max(dot(v_Normal, u_lightDir), 0.0);

    //specular
    vec3 Reflect = reflect(-u_lightDir, v_Normal);
    vec3 ViewDir = normalize(u_viewPosition - FragPos);
    v_SpecularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 8.0) * 0.33;
}
