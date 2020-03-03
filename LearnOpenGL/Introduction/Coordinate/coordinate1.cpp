#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../CShader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

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
		MixValue = MixValue + 0.0001f;
		if (MixValue >= 1.0f)
			MixValue = 1.0f;
	}
	if (glfwGetKey(vWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		MixValue = MixValue - 0.0001f;
		if (MixValue <= 0.0f)
			MixValue = 0.0f;
	}
}
//跟随时间旋转的正方体
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
	CShader ShaderProgram("Shader/Vertex/VertexSource9.vert", "Shader/Fragment/FragmentSource5.frag");
	//位置 纹理坐标
	float Vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//创建应用纹理
	unsigned int Texture1, Texture2;
	glGenTextures(1, &Texture1);
	glBindTexture(GL_TEXTURE_2D, Texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int Width, Height, NrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *Data = stbi_load("container.jpg", &Width, &Height, &NrChannels, 0);
	if (Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(Data);
	//第二张纹理
	glGenTextures(1, &Texture2);
	glBindTexture(GL_TEXTURE_2D, Texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);
	Data = stbi_load("awesomeface.png", &Width, &Height, &NrChannels, 0);
	if (Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(Data);
	ShaderProgram.useProgram();
	ShaderProgram.setInt("texture1", 0);
	ShaderProgram.setInt("texture2", 1);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(Window))
	{
		processInput(Window);
		ShaderProgram.useProgram();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		ShaderProgram.setFloat("MixValue", MixValue);
		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::rotate(Model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 View = glm::mat4(1.0f);
		View = glm::translate(View, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 Projection = glm::mat4(1.0f);
		Projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		int ModelLoc = glGetUniformLocation(ShaderProgram.m_ShaderId, "Model");
		int ViewLoc = glGetUniformLocation(ShaderProgram.m_ShaderId, "View");
		int ProLoc = glGetUniformLocation(ShaderProgram.m_ShaderId, "Projection");
		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(Model));
		glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(ProLoc, 1, GL_FALSE, glm::value_ptr(Projection));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}