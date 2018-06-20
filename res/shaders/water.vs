#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;

uniform mat4 u_projectionView;
uniform vec3 u_lightDir;
uniform vec3 u_viewPosition;
uniform vec4 u_frustumPlanes[4]; //back and front planes sucks!

out vec3  v_SkyboxCoords;
out float v_DiffuseComponent;
out float v_SpecularComponent;
out int   v_visible;
out vec3  v_FragPos;

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
    v_FragPos = i_pos;

    //diffuse
    v_DiffuseComponent = max(dot(i_normal, u_lightDir), 0.0);

    //specular
    vec3 ViewDir = normalize(u_viewPosition - i_pos);
    vec3 Reflect = reflect(-u_lightDir, i_normal);
    v_SpecularComponent = pow(max(dot(Reflect, ViewDir), 0.0), 16.0) * 0.375;

    //reflect skybox component
    v_SkyboxCoords = reflect(-ViewDir, normalize(0.25 * i_normal + vec3(0.0, 1.0, 0.0)));
}
