#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
void callbackFramebufferSize(GLFWwindow* vWindow, int vWidth, int vHeight)
{
	glViewport(0, 0, vWidth, vHeight);
}
void processInput(GLFWwindow* vWindow)
{
	if (glfwGetKey(vWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(vWindow, true);
	}
}
const char *VertexShaderSource =
"#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"	gl_PointSize = 10;\n"
"}\0";
const char *FragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\n\0";
const char *FragmentShaderSourceYe =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor=vec4(1.0f,1.0f,0.0f,1.0f);\n"
"}\n\0";

//创建两个着色器程序，第二个程序使用一个不同的片段着色器，输出黄色；
//再次绘制这两个三角形，让其中一个输出为黄色
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *Window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "failed to create window" << std::endl;
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
	int VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(VertexShader);
	int FragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader1, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShader1);
	int FragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader2, 1, &FragmentShaderSourceYe, NULL);
	glCompileShader(FragmentShader2);
	int ShaderProgram1 = glCreateProgram();
	glAttachShader(ShaderProgram1, VertexShader);
	glAttachShader(ShaderProgram1, FragmentShader1);
	glLinkProgram(ShaderProgram1);
	int ShaderProgram2 = glCreateProgram();
	glAttachShader(ShaderProgram2, VertexShader);
	glAttachShader(ShaderProgram2, FragmentShader2);
	glLinkProgram(ShaderProgram2);
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader1);
	glDeleteShader(FragmentShader2);
	float Vertices1[] =
	{
		0.0f,0.5f,0.0f,
		0.0f,0.0f,0.0f,
		-0.5f,0.0f,0.0f
	};
	float Vertices2[] =
	{
		0.0f,0.5f,0.0f,
		0.0f,0.0f,0.0f,
		0.5f,0.0f,0.0f
	};
	unsigned int VAO[2], VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1), Vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	while (!glfwWindowShouldClose(Window))
	{
		processInput(Window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(ShaderProgram1);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(ShaderProgram2);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glfwTerminate();
	return 0;
}