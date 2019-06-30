#pragma once

#include "../pch.h"

namespace Shaders
{
	class GLSLShaderManager final
	{
	public:
		enum class SHADER_TYPE
		{
			FRAGMENT,
			VERTEX,
			GEOMETRY
		};

		struct VertexShaderGLSL
		{
			std::string code;
			std::string path;
			std::string name;
			GLuint ID;
			bool isCompiled = false;
			bool isLoaded = false;

			void reset() { isCompiled = false; isLoaded = false; }
			void loadShader(const std::string& filePath, const std::string& optionalShaderName = "");
			void compileShader();
		};

		struct FragmentShaderGLSL
		{
			std::string code;
			std::string path;
			std::string name;
			GLuint ID;
			bool isCompiled = false;
			bool isLoaded = false;

			void reset() { isCompiled = false; isLoaded = false; }
			void loadShader(const std::string& filePath, const std::string& optionalShaderName = "");
			void compileShader();
		};

		struct Program
		{
			GLuint programID;
			std::string vertexShaderName;
			std::string fragmentShaderName;
			std::string programName;
			VertexShaderGLSL vertexShader;
			FragmentShaderGLSL fragShader;
		};

		using SharedPtr = std::shared_ptr<GLSLShaderManager>;

		static SharedPtr get() { return SharedPtr(new GLSLShaderManager); }

		VertexShaderGLSL			NULL_VERT_SHADER;
		FragmentShaderGLSL		    NULL_FRAG_SHADER;
		static const GLuint		    INVALID_PROGRAM = -1;

		VertexShaderGLSL&   getVertexShaderByName(const std::string& name);
		FragmentShaderGLSL& getFragmentShaderByName(const std::string& name);
		bool					  addVertexShader(const std::string& filePath, const std::string& optionalShaderName = "");
		bool					  addFragmentShader(const std::string& filePath, const std::string& optionalShaderName = "");
		bool					  compileShader(const std::string& shaderName, SHADER_TYPE);
		//this will be needed to be modified when the engine supports more than just vertex and fragment shaders
		GLuint					  createProgram(const std::string& programName, const std::string& vertShaderName, const std::string& fragShaderName);

	private:
		std::map<std::string, VertexShaderGLSL>   m_vertexShaders;
		std::map<std::string, FragmentShaderGLSL> m_fragmentShaders;
		std::map<std::string, FragmentShaderGLSL> m_programs;
	};
};

