#include "shader.hpp"
#include <iostream>
#include <fstream>

using namespace application::opengl;

Shader::Shader()
{
    _totalShaders=0;
    _shaders[VERTEX_SHADER]=0;
    _shaders[FRAGMENT_SHADER]=0;
    _shaders[GEOMETRY_SHADER]=0;
    _attributeList.clear();
    _uniformLocationList.clear();
}

Shader::~Shader()
{
    _attributeList.clear();
    _uniformLocationList.clear();
}

bool Shader::LoadFromString(GLenum whichShader, const std::string_view source)
{
    if (source.empty())
    {
        return false;
    }

    GLuint shader = glCreateShader (whichShader);

    GLint size = source.size();
    const char * ptmp = source.data();

    glShaderSource (shader, 1, &ptmp, &size);

    //check whether the shader loads fine
    GLint status;
    glCompileShader (shader);
    glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
            GLint infoLogLength;
            glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *infoLog= new GLchar[infoLogLength];
            glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog);
            std::cerr << "Compile log: " << infoLog << std::endl;
            delete [] infoLog;
    }
    _shaders[_totalShaders++] = shader;
    return true;
}

bool Shader::LoadFromFile(GLenum whichShader, const std::string &filename)
{
    std::ifstream fp(filename.c_str(), std::ios_base::in);
    if(fp)
    {
        std::string line, buffer;
        while(std::getline(fp, line))
        {
            buffer.append(line);
            buffer.append("\r\n");
        }
        //copy to source
        return LoadFromString(whichShader, buffer);
    }
    else
    {
        std::cerr << "Error loading shader: " << filename << std::endl;
        return false;
    }
}

void Shader::CreateAndLinkProgram()
{
    _program = glCreateProgram ();
    if (_shaders[VERTEX_SHADER] != 0) {
            glAttachShader (_program, _shaders[VERTEX_SHADER]);
    }
    if (_shaders[FRAGMENT_SHADER] != 0) {
            glAttachShader (_program, _shaders[FRAGMENT_SHADER]);
    }
    if (_shaders[GEOMETRY_SHADER] != 0) {
            glAttachShader (_program, _shaders[GEOMETRY_SHADER]);
    }

    //link and check whether the program links fine
    GLint status;
    glLinkProgram (_program);
    glGetProgramiv (_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
            GLint infoLogLength;

            glGetProgramiv (_program, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *infoLog= new GLchar[infoLogLength];
            glGetProgramInfoLog (_program, infoLogLength, NULL, infoLog);
            std::cerr << "Link log: " << infoLog << std::endl;
            delete [] infoLog;
    }

    glDeleteShader(_shaders[VERTEX_SHADER]);
    glDeleteShader(_shaders[FRAGMENT_SHADER]);
    glDeleteShader(_shaders[GEOMETRY_SHADER]);
}

void Shader::Use()
{
    glUseProgram(_program);
}

void Shader::UnUse()
{
    glUseProgram(0);
}

void Shader::AddAttribute(const std::string &attribute)
{
    _attributeList[attribute]= glGetAttribLocation(_program, attribute.c_str());
}

void Shader::AddUniform(const std::string &uniform)
{
    _uniformLocationList[uniform] = glGetUniformLocation(_program, uniform.c_str());
}

GLuint Shader::operator[](const std::string &attribute)
{
    return _attributeList[attribute];
}

GLuint Shader::operator()(const std::string &uniform)
{
    return _uniformLocationList[uniform];
}

void Shader::DeleteShaderProgram()
{
    glDeleteProgram(_program);
}
