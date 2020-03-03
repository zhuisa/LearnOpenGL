#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


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
const char *VertexsShaderSource1 =
"#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position=vec4(aPos,1.0f);\n"
"}\n\0";
const char *FragmentShaderSource1 =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 OurColor;\n"
"void main()\n"
"{\n"
"	FragColor=OurColor;\n"
"}\n\0";
//着色器使用uniform传递数据
//绿色-黑色-绿色
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
		std::cout << "failed to initialze glad" << std::endl;
		return -1;
	}
	int VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexsShaderSource1, NULL);
	glCompileShader(VertexShader);
	int Success;
	char InfoLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
		std::cout << "error::shader::vertex::compilation failed\n" << InfoLog << std::endl;
	}
	int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource1, NULL);
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
		std::cout << "error::shader::fragment::compilation failed\n" << InfoLog << std::endl;
	}
	int ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
		std::cout << "error::shader::program::linking failed\n" << InfoLog << std::endl;
	}
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	float Vertices[] =
	{
		0.0f,0.5f,0.0f,
		-0.5f,0.0f,0.0f,
		0.5f,0.0f,0.0f
	};
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	while (!glfwWindowShouldClose(Window))
	{
		processInput(Window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(ShaderProgram);
		float TimeValue = glfwGetTime();
		float GreenValue = sin(TimeValue) / 2.0f + 0.5f;
		int ColorLocation = glGetUniformLocation(ShaderProgram, "OurColor");
		glUniform4f(ColorLocation, 0.0f, GreenValue, 0.0f, 1.0f);
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
