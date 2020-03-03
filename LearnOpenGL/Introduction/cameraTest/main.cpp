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
glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float DeltaTime = 0.0f;
float LastFrame = 0.0f;
float LastX = 400;
float LastY = 300;
float Yaw = -90.0f;
float Pitch = 0.0f;
bool FirstMouse = true;
float Fov = 45.0f;
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
	float CurrentFrame = glfwGetTime();
	DeltaTime = CurrentFrame - LastFrame;
	LastFrame = CurrentFrame;
	float CameraSpeed = 2.5f*DeltaTime;
	if (glfwGetKey(vWindow, GLFW_KEY_W) == GLFW_PRESS)
		CameraPos += CameraSpeed * CameraFront;
	if (glfwGetKey(vWindow, GLFW_KEY_S) == GLFW_PRESS)
		CameraPos -= CameraSpeed * CameraFront;
	if (glfwGetKey(vWindow, GLFW_KEY_A) == GLFW_PRESS)
		CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp))*CameraSpeed;
	if (glfwGetKey(vWindow, GLFW_KEY_D) == GLFW_PRESS)
		CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp))*CameraSpeed;
}
void callbackMouse(GLFWwindow* vWindow, double xPos, double yPos)
{
	if (FirstMouse)
	{
		LastX = xPos;
		LastY = yPos;
		FirstMouse = false;
	}
	float XOffset = xPos - LastX;
	float YOffset = LastY - yPos;
	LastX = xPos;
	LastY = yPos;
	float Sensitivity = 0.05f;
	XOffset *= Sensitivity;
	YOffset *= Sensitivity;
	Yaw += XOffset;
	Pitch += YOffset;
	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;
	glm::vec3 Front;
	Front.x = cos(glm::radians(Yaw))*cos(glm::radians(Pitch));
	Front.y = sin(glm::radians(Pitch));
	Front.z = sin(glm::radians(Yaw))*cos(glm::radians(Pitch));
	CameraFront = glm::normalize(Front);
}
void callbackScroll(GLFWwindow *vWindow, double xoffset, double yoffset)
{
	if (Fov >= 1.0f && Fov <= 45.0f)
		Fov -= yoffset;
	if (Fov <= 1.0f)
		Fov = 1.0f;
	if (Fov >= 45.0f)
		Fov = 45.0f;
}
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
	glfwSetCursorPosCallback(Window, callbackMouse);
	glfwSetScrollCallback(Window, callbackScroll);
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
	glm::vec3 CubePosition[] =
	{
		glm::vec3(0.0f,  0.0f, 0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
		glm::mat4 View = glm::mat4(1.0f);
		float Radius = 10.0f;
		float CameX = sin(glfwGetTime())*Radius;
		float cameZ = cos(glfwGetTime())*Radius;
		View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
		//View = glm::lookAt(glm::vec3(CameX, 0.0f, cameZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Projection = glm::mat4(1.0f);
		//Projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		Projection = glm::perspective(glm::radians(Fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		ShaderProgram.setMat4("View", View);
		ShaderProgram.setMat4("Projection", Projection);
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 Model = glm::mat4(1.0f);
			Model = glm::translate(Model, CubePosition[i]);
			float Angle = 20.0f*i;
			Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ShaderProgram.setMat4("Model", Model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}