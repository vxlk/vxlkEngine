#pragma once
#include "../pch.h"
#include "Object.h"
#include "../Camera.h"
/*
A world contains :
Skybox
Lights
Objects
*/

//going to want to wrap a lot of rendering into the world render function eventually..

class glRenderer;
class dxRenderer;

class World final
{
	std::map<std::string, GLuint> m_lights;
	std::map<std::string, Object> m_objects;
	std::map<std::string, Camera*> m_cameras;
	std::string m_sceneName;

public:
	World() : m_sceneName("default"), m_objects(), m_cameras(), m_lights() { this->init(); }
	World(const std::string& name) : m_sceneName(name), m_objects(), m_cameras(), m_lights() { this->init(); }

	void setSceneName(const std::string& name) { m_sceneName = name; }

	bool init();
	void render(glRenderer* renderContext);
	void render(dxRenderer* renderContext);
	void addGL3rdPersonCamera(const std::string& name, GLuint programID);
	void addObject(const std::string& name, const std::string& filePath);
	Object& getObject(const std::string& name);
	Camera* getCamera(const std::string& name);
	void    addLight (const std::string& name, GLuint program);
	GLuint  getLight (const std::string& name);

};