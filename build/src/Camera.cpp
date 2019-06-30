#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
/*
GL Third Person
*/
void glThirdPersonCamera::init(GLuint programID)
{
	m_matrixID = glGetUniformLocation(programID, "MVP");
	m_viewMatrixID = glGetUniformLocation(programID, "V");
	m_modelMatrixID = glGetUniformLocation(programID, "M");
}

void glThirdPersonCamera::render()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();
	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	/* <Compute Matrices> */
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(m_verticalAngle) * sin(m_horizontalAngle),
		sin(m_verticalAngle),
		cos(m_verticalAngle) * cos(m_horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);
	/* </Compute Matrices> */

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m_projectionMatrix = glm::perspective(glm::radians(m_initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	m_viewMatrix = glm::lookAt(
		m_position,           // Camera is here
		m_position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}