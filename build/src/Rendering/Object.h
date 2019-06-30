#include "../pch.h"
#include "Texture.h"

enum class OBJECT_TYPE
{
	OBJ
};

struct Object
{

	//the texture to be loaded on the object
	Texture						m_texture;

	//Object Params
	std::vector<Vector3>		m_vertices;
	std::vector<Vector2>		m_uvs;
	std::vector<Vector3>		m_normals;
	
	//VBO index
	std::vector<unsigned short> m_indices;
	std::vector<Vector3>		m_indexed_vertices;
	std::vector<Vector2>		m_indexed_uvs;
	std::vector<Vector3>		m_indexed_normals;

	//object coords
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	GLuint ModelMatrixID;
	glm::mat4 MVP;

	void setModelMatrixID_GL(const GLuint& programID);

	bool indexIsOkay = false; //set true if VBO has been indexed
	bool loadedOkay  = false; //set true if successfully loaded

	static Object NULL_OBJECT;
};

/*
Wrapper class for all types of objects -> CAN ONLY CONTAIN ONE TYPE AT A TIME
you can then load the object into the wrapper and use it in the code -> it sets the type on construction
this avoids making multiple functions for every type of objects -> they will all
be handled in one class
*/
/*
template <typename ObjectType>
class Object
{
	std::shared_ptr<ObjectType> m_obj;
	OBJECT_TYPE m_typeOfObject; //set on construction

public:
	
	Object();

	constexpr OBJECT_TYPE getObjectType() { return m_typeOfObject; }

	std::shared_ptr<ObjectType> get() const { return m_obj; }

};
*/
class ObjLoader
{

public:
	/* TODO: after indexing -> clear vector of unindexed verts*/
	static Object			 loadOBJ(const std::string& filePath);
	static void				 indexVBO(Object& objectToBeMODIFIED);
};