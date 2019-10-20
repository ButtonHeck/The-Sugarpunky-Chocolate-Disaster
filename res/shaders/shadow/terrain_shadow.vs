#version 450

layout (location = 0) in vec4 i_pos;
layout (location = 2) in vec3 i_normal;

uniform int u_terrainType;

void main()
{
	vec4 pos = i_pos;
	if (u_terrainType == 0)
	{
		//slightly change y coord for smoother shadows at surface edges
		pos.y -= 0.06;
		/*
		in addition if a fragment is lower than the ground level, make it even lower,
		this leads to effect of hills being solid instead of wireframe, which leads
		to correct shadowing for shore
		*/
		if (pos.y < 0)
		    pos.y -= 5.0;
		gl_Position = pos;
	}
	else if (u_terrainType == 1)
	{
		//slightly change y coord for smoother shadows at surface edges
		pos.y -= 0.045;
		//for shore move fragment on the opposite of the normal X direction 
		if (i_normal.x > 0)
			pos.x -= 0.08;
		else
			pos.x += 0.08;
		gl_Position = pos;
	}
}
