#include "World.h"
#include "glRenderer.h"

bool World::init()
{
	//get cached objects, lights, and cameras from program init file
	return true;
}

void World::render(glRenderer* renderContext)
{
	

	//calculate mvp for object

}

void World::addGL3rdPersonCamera(const std::string& name, GLuint programID)
{
	m_cameras.emplace(name, new glThirdPersonCamera(programID));
	
}

void World::addObject(const std::string& name, const std::string& filePath)
{
	//m_objects.insert(std::make_pair("suzanne", ObjLoader::loadOBJ("C:/Users/small/Desktop/ogl-master_/tutorial09_vbo_indexing/suzanne.obj")));
	m_objects.emplace(name, ObjLoader::loadOBJ(filePath));
}

Object& World::getObject(const std::string& name)
{
	auto objectFound = m_objects.find(name);
	if (objectFound == m_objects.end())
		return Object::NULL_OBJECT;
	return m_objects[name];
}

Camera* World::getCamera(const std::string& name)
{
	auto objectFound = m_cameras.find(name);
	if (objectFound != m_cameras.end())
		return objectFound->second;
	return nullptr;
}

void World::addLight(const std::string& name, GLuint program)
{
	m_lights.emplace(name, glGetUniformLocation(program, name.c_str()));
}
GLuint World::getLight(const std::string& name)
{
	auto objectFound = m_lights.find(name);
	if (objectFound != m_lights.end())
		return objectFound->second;
	return INVALID_GLUINT;
}