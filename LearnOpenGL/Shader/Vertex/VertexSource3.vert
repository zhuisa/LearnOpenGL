#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 Color;
uniform float Distance;
out vec4 OurColor;
void main()
{
	gl_Position = vec4(aPos.x + Distance, aPos.y, aPos.z, 1.0f);
	OurColor = vec4(Color, 1.0f);
}