#pragma once
#include <string>
#include <GL\glew.h>
namespace Bengine{
	class GLSLProgram
	{
	public:
		GLSLProgram(void);
		~GLSLProgram(void);

		void compileShaders(const std::string& vertexShaderFilePath,const std::string& fragmentShaderFilePath);
		void linkShaders();
		//Add attribute of the shader
		void addAttribute(const std::string &attributeName);

		//get uniform variable in glsl 
		GLint getUniformLocation(const std::string& uniformName);

		//Use and unuse program
		void use();
		void unuse();

	private:
		int _numAttributes;
		GLuint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

		void compileShader(const std::string&  filePath,GLuint id);


	};
}
