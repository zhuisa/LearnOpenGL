#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../CShader.h"


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
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
}
//自己创建一个Shader类，方便以后使用
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
	CShader ShaderProgram("Shader/Vertex/VertexSource1.vert", "Shader/Fragment/FragmentSource1.frag");
	float Vertices[] =
	{
		0.0f,0.5f,0.0f,1.0f,0.0f,0.0f,
		-0.5f,0.0f,0.0f,0.0f,1.0f,0.0f,
		0.5f,0.0f,0.0f,0.0f,0.0f,1.0f
	};
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	while (!glfwWindowShouldClose(Window))
	{
		processInput(Window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ShaderProgram.useProgram();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}