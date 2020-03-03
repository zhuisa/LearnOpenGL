#pragma once
#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

class CShader
{
public:
	unsigned int m_ShaderId;
	CShader(const char* VertexPath, const char* FragmentPath)
	{
		std::string VertexCode, FragmentCode;
		//着色器文件
		std::ifstream VertexShaderFile, FragmentShaderFile;
		//确保可以抛出异常
		VertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		FragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//打开文件
			VertexShaderFile.open(VertexPath);
			FragmentShaderFile.open(FragmentPath);
			//读取文件缓冲区的内容到stream流中
			std::stringstream VertexShaderStream, FragmentShaderStream;
			VertexShaderStream << VertexShaderFile.rdbuf();
			FragmentShaderStream << FragmentShaderFile.rdbuf();
			//关闭文件
			VertexShaderFile.close();
			FragmentShaderFile.close();
			//将stream流的内容转换成字符串类型，得到着色器代码
			VertexCode = VertexShaderStream.str();
			FragmentCode = FragmentShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "error::shader failed to open shader file" << std::endl;
		}
		const char *VertexShaderCode = VertexCode.c_str();
		const char *FragmentShaderCode = FragmentCode.c_str();
		//编译着色器
		unsigned int VertexShader, FragmentShader;
		//编译顶点着色器
		VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
		glCompileShader(VertexShader);
		checkCompileErrors(VertexShader, "VERTEX");
		//编译片段着色器
		FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
		glCompileShader(FragmentShader);
		checkCompileErrors(FragmentShader, "FRAGMENT");
		//编译着色器程序
		m_ShaderId = glCreateProgram();
		glAttachShader(m_ShaderId, VertexShader);
		glAttachShader(m_ShaderId, FragmentShader);
		glLinkProgram(m_ShaderId);
		checkCompileErrors(m_ShaderId, "PROGRAM");
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
	}
	void useProgram()
	{
		glUseProgram(m_ShaderId);
	}

	void setBool(const std::string &vName, bool Value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderId, vName.c_str()), (int)Value);
	}
	void setInt(const std::string &vName, int Value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderId, vName.c_str()), Value);
	}
	void setFloat(const std::string &vName, float Value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderId, vName.c_str()), Value);
	}
	void setVec4(const std::string &vName, const glm::vec4 &Value) const
	{
		glUniform4fv(glGetUniformLocation(m_ShaderId, vName.c_str()), 1, &Value[0]);
	}
	void setMat4(const std::string &vName, glm::mat4 &Value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderId, vName.c_str()), 1, GL_FALSE, &Value[0][0]);
	}
private:
	void checkCompileErrors(unsigned int vShader, std::string vType)
	{
		int Success;
		char InfoLog[1024];
		if (vType != "PROGRAM")
		{
			glGetShaderiv(vShader, GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				glGetShaderInfoLog(vShader, 1024, NULL, InfoLog);
				std::cout << "error::shader::complie " << vType << "\n" << InfoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(m_ShaderId, GL_LINK_STATUS, &Success);
			if (!Success)
			{
				glGetProgramInfoLog(m_ShaderId, 1024, NULL, InfoLog);
				std::cout << "error::shader::link " << vType << "\n" << InfoLog << std::endl;
			}
		}
	}
};

