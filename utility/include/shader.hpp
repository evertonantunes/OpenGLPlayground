#pragma once

#include <GL/glew.h>
#include <string>
#include <map>

namespace application
{
    namespace opengl
    {
        class Shader
        {
        public:
            Shader(void);
            ~Shader(void);
            bool LoadFromString(GLenum whichShader, const std::string_view source);
            bool LoadFromFile(GLenum whichShader, const std::string& filename);
            void CreateAndLinkProgram();
            void Use();
            void UnUse();
            void AddAttribute(const std::string& attribute);
            void AddUniform(const std::string& uniform);

            //An indexer that returns the location of the attribute/uniform
            GLuint operator[](const std::string& attribute);
            GLuint operator()(const std::string& uniform);
            void DeleteShaderProgram();

        private:
            enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
            GLuint _program;
            int _totalShaders;
            GLuint _shaders[3];
            std::map<std::string, GLuint> _attributeList;
            std::map<std::string, GLuint> _uniformLocationList;
        };
    }
}
