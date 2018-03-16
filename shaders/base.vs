#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in mat4 instanceModel;

uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 FragPos;
out float PosHeight;

void main()
{
    gl_Position = projection * view * instanceModel * vec4(pos, 1.0);
    TexCoords = texCoords;
    FragPos = vec3(instanceModel * vec4(pos, 1.0));
    PosHeight = pos.y;
}
