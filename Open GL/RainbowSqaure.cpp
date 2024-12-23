/*
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

#include<fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum err = glGetError()) {
		std::string error = "";

		switch (err)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		printf("[Open GL Error] %d %s at Line No %d\n In Function: %s\nUsed in File: %s\n", err, error.c_str(), line, function, file);
		return false;
	}
	return true;
}

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& path) {
	std::ifstream stream(path);

	enum class Shadertype {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::string ans[2] = { "","" };
	Shadertype type = Shadertype::NONE;

	while (getline(stream, line, '\r')) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("fragment") != std::string::npos)
				type = Shadertype::FRAGMENT;
			else if (line.find("vertex") != std::string::npos)
				type = Shadertype::VERTEX;
		}
		else {
			ans[(int)type] += line + '\n';
		}
	}
	return { ans[0],ans[1] };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
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

		printf("%s Shader did not compile\n", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));
		printf("%s", message);

		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& verShade, const std::string& fragShade) {
	unsigned int pro = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, verShade);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShade);

	glAttachShader(pro, vs);
	glAttachShader(pro, fs);
	glLinkProgram(pro);
	glValidateProgram(pro);

	glDeleteShader(vs);
	glDeleteShader(fs);

	//glDetachShader(pro, vs);
	//glDetachShader(pro,fs);
	return pro;
}

int colorState = 0;
float red = 1.0f, green = 0.0f, blue = 0.0f;
void updateColors() {
	// State machine to cycle through rainbow colors
	float colorSpeed = 0.05f;
	switch (colorState) {
	case 0: // Red to Yellow
		green += colorSpeed;
		if (green >= 1.0f) colorState = 1;
		break;
	case 1: // Yellow to Green
		red -= colorSpeed;
		if (red <= 0.0f) colorState = 2;
		break;
	case 2: // Green to Cyan
		blue += colorSpeed;
		if (blue >= 1.0f) colorState = 3;
		break;
	case 3: // Cyan to Blue
		green -= colorSpeed;
		if (green <= 0.0f) colorState = 4;
		break;
	case 4: // Blue to Magenta
		red += colorSpeed;
		if (red >= 1.0f) colorState = 5;
		break;
	case 5: // Magenta to Red
		blue -= colorSpeed;
		if (blue <= 0.0f) colorState = 0;
		break;
	}
}

int main() {
	glfwInit();

	GLFWwindow* win = NULL;
	win = glfwCreateWindow(1920, 1080, "Cute Navanya", NULL, NULL);

	if (win == NULL) {
		printf("Null");
		return -1;
	}

	glfwMakeContextCurrent(win);

	glfwSwapInterval(2);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("GLAD not initialized");
		return -1;
	}

	float vert[8] = {
			-0.5f,-0.5f,
			0.5f,-0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f,
	};
	unsigned int ind[6] = {
		0,1,2,
		0,2,3
	};

	unsigned int a;
	glGenBuffers(1, &a);
	glBindBuffer(GL_ARRAY_BUFFER, a);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vert, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	unsigned int b;
	glGenBuffers(1, &b);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), ind, GL_STATIC_DRAW);

	ShaderProgramSource src = ParseShader("res/Basic.shader");

	unsigned int shader = CreateShader(src.VertexSource, src.FragmentSource);
	glUseProgram(shader);

	GLCall(int c = glGetUniformLocation(shader, "n_color"));
	ASSERT(c != -1);

	GLCall(glUniform4f(c, red, green, blue, 1.0f));

	while (!glfwWindowShouldClose(win))
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		updateColors();
		GLCall(glUniform4f(c, red, green, blue, 1.0f));

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}
*/