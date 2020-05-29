#version 450

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_texCoords;
layout (location = 2) in vec3 i_normal;
layout (location = 3) in vec3 i_tangent;
layout (location = 4) in vec3 i_bitangent;

out vec3 v_pos;
out vec2 v_texCoords;
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_bitangent;
//v_visible - define whether a vertex should be emitted at the geometry stage
out int  v_visible;

uniform vec4 u_frustumPlanes[5];
const float CULLING_OFFSET = 2;

bool frustumCulling( vec3 position )
{
    for( int planeIndex = 0; planeIndex < u_frustumPlanes.length(); planeIndex++ )
    {
        if( dot( u_frustumPlanes[planeIndex].xyz, position ) <= -u_frustumPlanes[planeIndex].w - CULLING_OFFSET )
		{
			return false;
		}
    }
	return true;
}

void main()
{
    v_pos = i_pos;
    v_texCoords = i_texCoords;
    v_normal = i_normal;
    v_tangent = i_tangent;
    v_bitangent = i_bitangent;
    if( frustumCulling(i_pos) )
	{
        v_visible = 1;
	}
    else
	{
        v_visible = 0;
	}
}
