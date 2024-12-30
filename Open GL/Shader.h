#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
	std::string CompSource;
};

class Shader
{
private:
	std::string n_FilePath;
	std::unordered_map <std::string, int> m_locationCache;
	unsigned int m_RendererID;
public:
	Shader(const std::string& path);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void SetUniform1i(const std::string& name, int i1);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4(const std::string& name,glm::mat4 mat);

private:
	ShaderProgramSource ParseShader(const std::string& path);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& verShade, 
		const std::string& fragShade, const std::string& CompShade="");

	int GetUniformLocation(const std::string& name);
};

