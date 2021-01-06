#include "GLSLProgram.h"
#include "ErrorMess.h"
#include <vector>
#include <fstream>
namespace Bengine{
	GLSLProgram::GLSLProgram(void):_numAttributes(0),_programID(0),_vertexShaderID(0),_fragmentShaderID(0)
	{

	}


	GLSLProgram::~GLSLProgram(void)
	{

	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath,const std::string& fragmentShaderFilePath)
	{
		_programID = glCreateProgram();
		if(_programID == 0)
		{
			fatalError("Cannot create program!");
		}
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if(_vertexShaderID == 0 )
		{
			fatalError("Vertex shader failed to be created");
		}
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if(_fragmentShaderID == 0 )
		{
			fatalError("Fragment shader failed to be created");
		}
		compileShader(vertexShaderFilePath,_vertexShaderID);
		compileShader(fragmentShaderFilePath,_fragmentShaderID);

	}
	void  GLSLProgram::compileShader(const std::string&  filePath,GLuint id)
	{

		//Read GLSL file to get text and compile it 
		std::fstream gslfile(filePath);
		if(gslfile.fail())
		{
			perror(filePath.c_str());
			fatalError("Failed to open "+filePath);
		}

		std::string fileContents = "";
		std::string line;

		while(std::getline(gslfile,line))
		{
			fileContents += line + "\n";
		}

		gslfile.close();

		//tell openGL where to get the shader source
		const char* contentPtr = fileContents.c_str(); 
		glShaderSource(id,1,&contentPtr,nullptr);

		glCompileShader(id);

		//Error checking when compile shader
		GLint success =0;
		glGetShaderiv(id,GL_COMPILE_STATUS,&success);
		if(success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id,GL_INFO_LOG_LENGTH,&maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id,maxLength,&maxLength,&errorLog[0]);

			//Exit with failure
			glDeleteShader(id); //Don't leak the shader

			std::printf("%s\n",&(errorLog[0]));
			fatalError("Shader failed to compile!");

		}

	}
	void  GLSLProgram::linkShaders()
	{

		//Attach shader to proram
		glAttachShader(_programID,_vertexShaderID);
		glAttachShader(_programID,_fragmentShaderID);
		//link shader to program
		glLinkProgram(_programID);

		GLint isLinked =0;
		glGetProgramiv(_programID,GL_LINK_STATUS,(int*)&isLinked);

		//handle when linking error
		if(isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(_programID,GL_INFO_LOG_LENGTH,&maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(_programID,maxLength,&maxLength,&errorLog[0]);

			//Exit with failure
			glDeleteProgram(_programID); //Delete program 
			glDeleteShader(_vertexShaderID);//Delete the shader
			glDeleteShader(_fragmentShaderID);

			std::printf("%s\n",&(errorLog[0]));
			fatalError("Shader filed to link!");
		}
		//Always detach shader after succesful link 
		glDetachShader(_programID,_vertexShaderID);
		glDetachShader(_programID,_fragmentShaderID);
		//Delete shader after done
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}
	void GLSLProgram::addAttribute(const std::string &attributeName)
	{
		//bind attributes
		glBindAttribLocation(_programID,_numAttributes++,attributeName.c_str());
	}
	GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID,uniformName.c_str());
		if(location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}
	void GLSLProgram::use()
	{
		glUseProgram(_programID);
		//Enable attribute
		for(int i=0; i<=_numAttributes;i++)
		{
			glEnableVertexAttribArray(i);
		}

	}
	void GLSLProgram::unuse()
	{
		glUseProgram(0);
	}
}