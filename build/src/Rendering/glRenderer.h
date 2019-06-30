#pragma once
#include "../pch.h"
#include "Shader.h"
#include "World.h"

struct GLFWwindow;

class glRenderer
{
public:
	/*
	Object loaded into opengl's memory
	*/
	struct glObject
	{
		GLuint m_vertexBuffer = 0;
		GLuint m_uvBuffer = 0;
		GLuint m_normalBuffer = 0;
		GLuint m_elementBuffer = 0;
		GLuint m_program = 0;
		GLuint m_textID = 0;

		glObject() : m_vertexBuffer(0), m_uvBuffer(0), m_normalBuffer(0), m_elementBuffer(0), m_program(0), m_textID(0) {}
	};

	using SharedPtr = std::shared_ptr<glRenderer>;

	GLuint m_vertexBufferID;
	GLuint m_uvbufferID;
	GLuint m_normalBufferID;
	GLuint m_elementBufferID;

	//init the default scene
	bool init(const std::string& sceneName = "default");

	void render(GLFWwindow* windowToRenderTo);

	//also constructs;
	static SharedPtr get();

	static glObject NULL_GPU_OBJECT;

private:
	//for calculating fps
	double m_currentTime;
	double m_lastTime;
	unsigned int m_numFrames;
	GLuint m_currProg;

	Shaders::GLSLShaderManager      m_shaders;
	std::vector<GLuint>			    m_programs;
	std::map<std::string, glObject>	m_gpuObjects;
	std::vector<GLuint>		        m_shaderPrograms;
	World m_sceneToRender;

	void	  createGPUObject(Object& obj, const std::string& objName); //was const object
	glObject& getGPUObject   (const std::string& name);

};

