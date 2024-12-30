#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"


Shader::Shader(const std::string& path)
{
	ShaderProgramSource src = ParseShader(path);
	m_RendererID = CreateShader(src.VertexSource, src.FragmentSource, src.CompSource);
	n_FilePath = path;
}
Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));

}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int i1)
{
	GLCall(glUniform1i(GetUniformLocation(name), i1));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 mat)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name),1,GL_FALSE,&mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_locationCache.find(name)!=m_locationCache.end())
		return m_locationCache[name];

	GLCall(int c = glGetUniformLocation(m_RendererID, name.c_str()));
	if (c == -1) { std::cout<<"warning: uniform '"+name+"' doesnt exist\n"<<std::endl; }
	m_locationCache[name] = c;
	
	return c;
}


ShaderProgramSource Shader::ParseShader(const std::string& path) {
	std::ifstream stream(path);

	enum class Shadertype {
		NONE = -1, VERTEX = 0, FRAGMENT = 1, COMPUTE =2
	};

	std::string line;
	std::string ans[3] = { "","",""};
	Shadertype type = Shadertype::NONE;

	while (getline(stream, line, '\r')) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("fragment") != std::string::npos)
				type = Shadertype::FRAGMENT;
			else if (line.find("vertex") != std::string::npos)
				type = Shadertype::VERTEX;
			else if (line.find("compute") != std::string::npos)
				type = Shadertype::COMPUTE;
		}
		else {
			ans[(int)type] += line + '\n';
		}
	}
	return { ans[0],ans[1],ans[2]};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL); // HERE nullptr
	glCompileShader(id);

	int pass;
	glGetShaderiv(id, GL_COMPILE_STATUS, &pass);
	if (pass == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::string sh;
		if (type == GL_VERTEX_SHADER)
			sh = "Vertex";
		else if (type == GL_FRAGMENT_SHADER)
			sh = "Fragment";
		else
			sh = "Compute";
		std::cout<<sh+" Shader did not compile"<<std::endl;
		printf("%s", message);

		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& verShade, const std::string& fragShade, 
	const std::string& CompShade) {
	unsigned int pro = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, verShade);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShade);
	unsigned int cs;
	if (CompShade != "") {
		cs = CompileShader(GL_COMPUTE_SHADER, CompShade);
		glAttachShader(pro, cs);
	}

	glAttachShader(pro, vs);
	glAttachShader(pro, fs);
	glLinkProgram(pro);
	glValidateProgram(pro);

	glDeleteShader(vs);
	glDeleteShader(fs);
	if (CompShade != "") glDeleteShader(cs);

	//glDetachShader(pro, vs);
	//glDetachShader(pro,fs);
	return pro;
}
