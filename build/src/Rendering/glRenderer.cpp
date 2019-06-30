#include "glRenderer.h"
#include "../pch.h"
#include <iostream>

//static struct declaration
glRenderer::glObject glRenderer::NULL_GPU_OBJECT;

void glRenderer::render(GLFWwindow* windowToRenderTo)
{
	while (glfwGetKey(windowToRenderTo, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(windowToRenderTo) == 0)
	{
		// Measure speed
		m_currentTime = glfwGetTime();
		m_numFrames++;
		if (m_currentTime - m_lastTime >= 1.0) { // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0 / double(m_numFrames));
			m_numFrames = 0;
			m_lastTime += 1.0;
		}
		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute the MVP matrix from keyboard and mouse input
		m_sceneToRender.getCamera("mainCamera")->render();
		glm::mat4 ProjectionMatrix = m_sceneToRender.getCamera("mainCamera")->getProjMatrix();
		glm::mat4 ViewMatrix	   = m_sceneToRender.getCamera("mainCamera")->getViewMatrix();

		glUseProgram(m_currProg);


		//for each light -> illuminate
	    //for each object in scene -> render
		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(m_sceneToRender.getLight("main_light"), lightPos.x, lightPos.y, lightPos.z);
		glUniformMatrix4fv(m_sceneToRender.getCamera("mainCamera")->getViewMatrixID(), 1, GL_FALSE, &m_sceneToRender.getCamera("mainCamera")->getViewMatrix()[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "programID"
		//m_sceneToRender.render(this);

		//render object
		m_sceneToRender.getObject("suzanne").MVP = ProjectionMatrix * ViewMatrix * m_sceneToRender.getObject("suzanne").ModelMatrix;
		m_sceneToRender.getObject("suzanne").setModelMatrixID_GL(m_currProg); //assumes init was called!
	
		//render object!
		
		// in the "MVP" uniform
		glUniformMatrix4fv(m_sceneToRender.getCamera("mainCamera")->getMatrixID(), 1, GL_FALSE, &m_sceneToRender.getObject("suzanne").MVP[0][0]);
		glUniformMatrix4fv(m_sceneToRender.getObject("suzanne").ModelMatrixID, 1, GL_FALSE, &m_sceneToRender.getObject("suzanne").ModelMatrix[0][0]);


		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_sceneToRender.getObject("suzanne").m_texture.getID());
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(m_sceneToRender.getObject("suzanne").m_texture.getUniform(m_currProg, "texture1Uniform"), 0);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glRenderer::glObject objgl = getGPUObject("suzanne");
		glBindBuffer(GL_ARRAY_BUFFER, getGPUObject("suzanne").m_vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, getGPUObject("suzanne").m_uvBuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, getGPUObject("suzanne").m_normalBuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getGPUObject("suzanne").m_elementBuffer);

		auto size2 = m_sceneToRender.getObject("suzanne").m_indices.size();

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			m_sceneToRender.getObject("suzanne").m_indices.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(windowToRenderTo);
		glfwPollEvents();
	}
}

bool glRenderer::init(const std::string& sceneName)
{
	glGenVertexArrays(1, &m_vertexBufferID);
	glBindVertexArray(m_vertexBufferID);

	//init worldToRender
	m_sceneToRender.setSceneName(sceneName);
	m_sceneToRender.init();

	// Create and compile our GLSL program from the shaders
	if (!m_shaders.addVertexShader("ShaderBin/StandardShading.vertexshader", "standardVertex")) std::cout << "Vertex shader didnt load\n";
	if (!m_shaders.addFragmentShader("ShaderBin/StandardShading.fragmentshader", "standardFragment")) std::cout << "Fragment shader didnt load\n";
	m_shaders.compileShader("standardVertex", Shaders::GLSLShaderManager::SHADER_TYPE::VERTEX);
	m_shaders.compileShader("standardFragment", Shaders::GLSLShaderManager::SHADER_TYPE::FRAGMENT); //this might cause an issue and put the wrong program id
	m_programs.emplace_back(m_shaders.createProgram("standardProgram", "standardVertex", "standardFragment"));
	m_currProg = m_programs[m_programs.size() - 1];
	//add camera here
	m_sceneToRender.addGL3rdPersonCamera("mainCamera", m_currProg);
	dynamic_cast<glThirdPersonCamera *> (m_sceneToRender.getCamera("mainCamera"))->init(m_currProg);

	//add object here
	m_sceneToRender.addObject("suzanne", "C:/Users/small/Desktop/ogl-master_/tutorial09_vbo_indexing/suzanne.obj"); //not being indexed rn
	createGPUObject(m_sceneToRender.getObject("suzanne"), "suzanne");
	//load texture
	m_sceneToRender.getObject("suzanne").m_texture.loadDDS("C:/Users/small/Desktop/ogl-master_/tutorial09_vbo_indexing/uvmap.DDS", "uvmap");
	GLuint textID = glGetUniformLocation(m_sceneToRender.getObject("suzanne").m_texture.getID(), "uvmap"); ///find a way to do this better
	getGPUObject("suzanne").m_textID = textID;
	getGPUObject("suzanne").m_program = m_currProg;

	glUseProgram(m_currProg);

	m_sceneToRender.addLight("main_light", m_currProg);

	m_lastTime = glfwGetTime();
	m_numFrames = 0;

	//fix later
	return true;
}


void glRenderer::createGPUObject(Object& obj, const std::string& objName)
{
	glObject glObj;

	// Load it into a VBO
	glGenBuffers(1, &(glObj.m_vertexBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, glObj.m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, obj.m_indexed_vertices.size() * sizeof(glm::vec3), &obj.m_indexed_vertices, GL_STATIC_DRAW);

	//here
	glGenBuffers(1, &(glObj.m_uvBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, glObj.m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, obj.m_indexed_uvs.size() * sizeof(glm::vec2), &obj.m_indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &(glObj.m_normalBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, glObj.m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, obj.m_indexed_normals.size() * sizeof(glm::vec3), &obj.m_indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &(glObj.m_elementBuffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glObj.m_elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.m_indices.size() * sizeof(unsigned short), &obj.m_indices[0], GL_STATIC_DRAW);

	//add to list of objects in renderer
	m_gpuObjects.emplace(objName, glObj);
}

glRenderer::SharedPtr glRenderer::get()
{
	glRenderer::SharedPtr newRenderer;
	newRenderer.get()->init();
	return newRenderer;
}

glRenderer::glObject& glRenderer::getGPUObject(const std::string& name)
{
	std::map<std::string, glObject>::iterator objectFound = m_gpuObjects.find(name);
	if (objectFound == m_gpuObjects.end())
		return NULL_GPU_OBJECT;

	return m_gpuObjects[name];
}