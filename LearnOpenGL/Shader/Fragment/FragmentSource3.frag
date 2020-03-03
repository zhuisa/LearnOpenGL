#version 330 core
out vec4 FragColor;
in vec3 OurColor;
in vec2 TexCoord;
uniform sampler2D OurTexture;
void main()
{
	FragColor = texture(OurTexture, TexCoord)*vec4(OurColor,1.0f);
}