#include <own/shader.h>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

string ReadFileContent(const char* filename) {
	ifstream in(filename, ios::binary);
	if (in) {
		string content;
		in.seekg(0, ios::end);
		content.resize(in.tellg());
		in.seekg(0, ios::beg);
		in.read(&content[0], content.size());
		in.close();
		return (content);
	}
	cerr << "FAILED READING FILE" << endl;

}

GLuint CreateShader(const char* name) {

	GLint hasCompiled;
	std::string path = "res/shaders/" + std::string(name) + ".vsh";
	string vc = ReadFileContent(path.c_str());
	path = "res/shaders/" + std::string(name) + ".fsh";
	string fc = ReadFileContent(path.c_str());

	const char* vertexSource = vc.c_str();
	const char* fragmentSource = fc.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &hasCompiled);
	std::cout << "VERT: " << hasCompiled << std::endl;

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &hasCompiled);
	std::cout << "FRAG: " << hasCompiled << std::endl;

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &hasCompiled);
	std::cout << "LINK: " << hasCompiled << std::endl;

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;

};
