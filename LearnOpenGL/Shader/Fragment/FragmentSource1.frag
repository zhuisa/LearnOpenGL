#version 330 core
out vec4 FragColor;
in vec4 OurColor;
void main()
{
	FragColor = OurColor;
}