#pragma once

#include "pch.h"

class Camera
{
protected:
	// Initial position : on +Z
	glm::vec3 m_position = glm::vec3(0, 0, 5); //change this later maybe
	// Initial horizontal angle : toward -Z
	float m_horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float m_verticalAngle = 0.0f;
	// Initial Field of View
	float m_initialFoV = 45.0f;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	GLuint m_matrixID;
	GLuint m_viewMatrixID;
	GLuint m_modelMatrixID;


public:

	GLuint getMatrixID() const { return m_matrixID; }
	GLuint getViewMatrixID() const { return m_viewMatrixID; }
	GLuint getModelMatrixID() const { return m_modelMatrixID; }

	//these can be edited directly
	glm::mat4& getViewMatrix() { return m_viewMatrix; }
	glm::mat4& getProjMatrix() { return m_projectionMatrix; }

public:
	//call this every frame
	virtual void render() = 0;
};

class glCamera : public Camera
{
protected:
public:
	virtual void render() = 0;
};

class glThirdPersonCamera : public glCamera
{
public:
	void init(GLuint programID);
	//call this every frame
	void render() override;
	glThirdPersonCamera() = delete;
	glThirdPersonCamera(GLuint programID) { init(programID); }
};

