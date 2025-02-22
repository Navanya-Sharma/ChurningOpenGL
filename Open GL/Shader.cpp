#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"


Shader::Shader(const std::string& path)
{
	n_FilePath = path;
	ShaderProgramSource src = ParseShader(path);
	m_RendererID = CreateShader(src.VertexSource, src.FragmentSource, src.CompSource, src.GeoSource);
}
Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{

	GLCall(glUseProgram(m_RendererID));
	// If there is a variable mismatch between in and out of shaders, it gives an invalid operation error

}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int i1)
{
	Bind();
	GLCall(glUniform1i(GetUniformLocation(name), i1));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	Bind();
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	Bind();
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2 ));
}
void Shader::SetUniform1fv(const std::string& name, int count, float *data)
{
	Bind();
	GLCall(glUniform1fv(GetUniformLocation(name), count, data));
}
void Shader::SetUniformMat4(const std::string& name, glm::mat4 mat)
{
	Bind();
	GLCall(glUniformMatrix4fv(GetUniformLocation(name),1,GL_FALSE,&mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_locationCache.find(name)!=m_locationCache.end())
		return m_locationCache[name];

	GLCall(int c = glGetUniformLocation(m_RendererID, name.c_str()));
	if (c == -1) { std::cout<<n_FilePath+"warning: uniform '"+name+"' doesnt exist\n"<<std::endl; }
	m_locationCache[name] = c;
	
	return c;
}


ShaderProgramSource Shader::ParseShader(const std::string& path) {
	std::ifstream stream(path);

	if (stream.fail()) {
		std::cerr << "Error: Failed to open file at path: " << path << std::endl;
		ASSERT(0);
	}
	enum class Shadertype {
		NONE = -1, VERTEX = 0, FRAGMENT = 1, COMPUTE =2, GEOMETRY = 3
	};

	std::string line;
	std::string ans[4] = { "","","",""};
	Shadertype type = Shadertype::NONE;

	while (getline(stream, line, '\n')) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("fragment") != std::string::npos)
				type = Shadertype::FRAGMENT;
			else if (line.find("vertex") != std::string::npos)
				type = Shadertype::VERTEX;
			else if (line.find("compute") != std::string::npos)
				type = Shadertype::COMPUTE;
			else if (line.find("geometry") != std::string::npos)
				type = Shadertype::GEOMETRY;
		}
		else {
			ans[(int)type] += line + '\n';
		}
	}
	return { ans[0],ans[1],ans[2],ans[3]};
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
		else if(type == GL_GEOMETRY_SHADER)
			sh= "Geometry";
		else if(type == GL_COMPUTE_SHADER)
			sh = "Compute";
		std::cout<<sh+" Shader did not compile from path"+n_FilePath<<std::endl;
		printf("%s", message);

		glDeleteShader(id);
		ASSERT(0);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& verShade, const std::string& fragShade, 
	const std::string& CompShade, const std::string& GeoShade) {
	unsigned int pro = glCreateProgram();
	unsigned int vs,fs,cs,gs;
	if (verShade != "") {
		vs = CompileShader(GL_VERTEX_SHADER, verShade);
		glAttachShader(pro, vs);
	}
	if (fragShade !="") {
		fs = CompileShader(GL_FRAGMENT_SHADER, fragShade);
		glAttachShader(pro, fs);
	}
	if (CompShade != "") {
		cs = CompileShader(GL_COMPUTE_SHADER, CompShade);
		glAttachShader(pro, cs);
	}
	if (GeoShade != "") {
		gs = CompileShader(GL_GEOMETRY_SHADER, GeoShade);
		glAttachShader(pro, gs);
	}

	glLinkProgram(pro);
	glValidateProgram(pro);

	if (verShade != "") glDeleteShader(vs);
	if (fragShade != "") glDeleteShader(fs);
	if (CompShade != "") glDeleteShader(cs);
	if (GeoShade != "") glDeleteShader(gs);

	//glDetachShader(pro, vs);
	//glDetachShader(pro,fs);
	return pro;
}
