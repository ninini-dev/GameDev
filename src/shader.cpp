#include <iostream>
#include <own/shader.h>
#include <fstream>
#include <string>

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

GLuint CreateShader(const char* vert, const char* frag, const char* geom) {

    GLint hasCompiled;
    string vc = ReadFileContent(vert);
    string fc = ReadFileContent(frag);

    const char* vertexSource = vc.c_str(); 
    const char* fragmentSource = fc.c_str();

    GLuint shaderProgram = glCreateProgram();

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


    GLuint geomShader = 0;
    if (geom != "") {
        string gc = ReadFileContent(geom);
        const char* geomSrc = gc.c_str();
        GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geomShader, 1, &geomSrc, NULL);
        glCompileShader(geomShader);
        glGetShaderiv(geomShader, GL_COMPILE_STATUS, &hasCompiled);
        std::cout << "GEOM: " << hasCompiled << std::endl;
        glAttachShader(shaderProgram, geomShader);
    }
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &hasCompiled);
    std::cout << "LINK: " << hasCompiled << std::endl;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geomShader!=0)glDeleteShader(geomShader);
    return shaderProgram;

};