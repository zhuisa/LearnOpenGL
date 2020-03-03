#version 330 core
out vec4 FragColor;
in vec3 Dis;
void main()
{
	FragColor = vec4(Dis, 1.0f);
}
