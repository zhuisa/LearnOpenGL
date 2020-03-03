
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "..\CShader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\stb_image.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
float MixValue = 0.2f;
void callbackFramebufferSize(GLFWwindow *vWindow, int vWidth, int vHeight)
{
	glViewport(0, 0, vWidth, vHeight);
}
void processInput(GLFWwindow *vWindow)
{
	if (glfwGetKey(vWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(vWindow, true);
	}
	if (glfwGetKey(vWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		MixValue = MixValue + 0.001f;
		if (MixValue >= 1.0f)
			MixValue = 1.0f;
	}
	if (glfwGetKey(vWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		MixValue = MixValue - 0.001f;
		if (MixValue <= 0.0f)
			MixValue = 0.0f;
	}
}
//����ɫ�ľ��μ�����
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *Window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);
	glfwSetFramebufferSizeCallback(Window, callbackFramebufferSize);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize glad" << std::endl;
		return -1;
	}
	CShader ShaderProgram("Shader/Vertex/VertexSource5.vert", "Shader/Fragment/FragmentSource3.frag");
	//λ�á���ɫ����������
	float Vertices[] =
	{
		0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,0.0f,
	   -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,0.0f,
	   -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,1.0f
	};
	unsigned int Indexs[] =
	{
		0,1,3,
		1,2,3
	};
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indexs), Indexs, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//�������ɹ���
	unsigned int Texture;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	//Ϊ��ǰ�󶨵��������������������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//���ز���������
	int Width, Height, nrChannels;
	//ͼ��·����ͼ����ȡ�ͼ��߶ȡ�ͼ����ɫͨ������
	unsigned char *Data = stbi_load("container.jpg", &Width, &Height, &nrChannels, 0);
	if (Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��������
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(Data);
	while (!glfwWindowShouldClose(Window))
	{
		processInput(Window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ShaderProgram.useProgram();
		glBindTexture(GL_TEXTURE_2D, Texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}