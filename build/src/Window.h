#pragma once

#include "pch.h"
#include "Rendering/glRenderer.h"

class Window
{
protected:
	unsigned int m_width;
	unsigned int m_height;
	Vector4		 m_backgroundColor;
	unsigned int m_windowID;
	ENGINE_TYPE  m_windowType;

public:
	Window() : m_width(1024), m_height(800), m_backgroundColor(0), m_windowID(-1), m_windowType(ENGINE_TYPE::GL_ENGINE) {}
	virtual ~Window() noexcept {}

	virtual void update() = 0;

	constexpr unsigned int getWindowWidth()  { return m_width; }
	constexpr unsigned int getWindowHeight() { return m_height; }
	constexpr unsigned int getWindowID()	 { return m_windowID; }
	constexpr ENGINE_TYPE  getWindowType()   { return m_windowType; }

	void setWindowID(unsigned int newID)	 { m_windowID = newID; }
};

class glWindow : public Window
{
	enum class errorMessage
	{
		glfwWindowFailure,
		glewInitFailure,
		glfwInitFailure,
		NoError
	};

	errorMessage init();

	/*The instance of the gl window*/
	std::function<void(GLFWwindow*)> glfwDeleter = [](GLFWwindow* ptr) { glfwDestroyWindow(ptr); }; //deleter utility
	std::unique_ptr<GLFWwindow, decltype(glfwDeleter)> m_glWindow; //smart pointer to window

	glRenderer m_renderContext;

public:
	glWindow();
	glWindow(const unsigned int& width, const unsigned int& height, const Vector4& backgroundColor);
	virtual ~glWindow() noexcept { glfwTerminate(); }

	virtual void update() override;
};

class dxWindow : public Window
{
public:
	dxWindow(const unsigned int& width, const unsigned int& height, const Vector4& backgroundColor) { m_windowType = ENGINE_TYPE::DX_ENGINE; }
	virtual void update() override {}
};

//allow for multiple windows in an application **Singleton**
class WindowManager
{
	//Collection of Window parent objects that automatically delete themselves when the manager goes
	//out of scope.  Allows for factory pattern without calling delete.
	std::vector<std::shared_ptr<Window>> m_windowCollection;

	unsigned int m_indexCount = 0; //this is okay as non static because this is a singleton class

public:
	WindowManager() { m_windowCollection.reserve(2); }
	~WindowManager()
	{
		m_windowCollection.clear();
	}

	static WindowManager& getInstance()
	{
		static WindowManager w;
		return w;
	}

	//returns id
	unsigned int			addWindow(
									   const unsigned int& width, 
									   const unsigned int& height, 
									   const Vector4& backgroundColor,
									   ENGINE_TYPE typeOfWindow = currentRenderingEngine
									  );

	bool					deleteWindow(unsigned int windowID);

	Window*					getWindowByID(unsigned int windowID);
};

inline unsigned int WindowManager::addWindow(
											 const unsigned int& width,
											 const unsigned int& height,
											 const Vector4& backgroundColor,
											 ENGINE_TYPE typeOfWindow
											)
{
	typeOfWindow == ENGINE_TYPE::GL_ENGINE
	?
	m_windowCollection.emplace_back(new glWindow(width, height, backgroundColor))
	:
	m_windowCollection.emplace_back(new dxWindow(width, height, backgroundColor)); //else make a dx window

	++m_indexCount;
	m_windowCollection[m_windowCollection.size() - 1]->setWindowID(m_indexCount);
	return m_indexCount;
}

inline bool WindowManager::deleteWindow(unsigned int windowID)
{
	for (auto i = 0; i < m_windowCollection.size(); ++i)
		if (m_windowCollection[i].get()->getWindowID() == windowID)
		{
			m_windowCollection.erase(m_windowCollection.begin() + i);
			return true;
		}
	return false;
}

inline Window* WindowManager::getWindowByID(unsigned int windowID)
{
	if (windowID == -1) fprintf(stderr, "Window not initialized - use WindowManager::addWindow to set an ID");
	for (auto i = 0; i < m_windowCollection.size(); ++i)
		if (m_windowCollection[i].get()->getWindowID() == windowID)
		{
			if (m_windowCollection[i].get()->getWindowType() == ENGINE_TYPE::GL_ENGINE)
				return dynamic_cast<glWindow*>(m_windowCollection[i].get());
			if (m_windowCollection[i].get()->getWindowType() == ENGINE_TYPE::DX_ENGINE)
				return dynamic_cast<dxWindow*>(m_windowCollection[i].get());
			if (m_windowCollection[i].get()->getWindowType() == ENGINE_TYPE::TWO_D_ENGINE) //FIX LATER
				return dynamic_cast<dxWindow*>(m_windowCollection[i].get());
		}
	return nullptr;
}