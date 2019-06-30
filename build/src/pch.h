#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include "config.h"

/*
	The engine will support multiple rendering windows ... this sets the default renderer based on option selected
*/

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

using Vector4 = vec4;
using Vector3 = vec3;
using Vector2 = vec2;

#ifdef VX_GL

	// Include GLEW
	#include "GL/glew.h"

	// Include GLFW
	#include "GLFW/glfw3.h"
	
	static constexpr GLuint INVALID_GLUINT = 999999;

	#ifndef RENDERING_PLATFORM
	#define RENDERING_PLATFORM
		enum class ENGINE_TYPE
		{
			GL_ENGINE,
			DX_ENGINE,
			TWO_D_ENGINE
		};
		static ENGINE_TYPE currentRenderingEngine = ENGINE_TYPE::GL_ENGINE;
	#endif
#endif

#ifdef VX_DX11
	#ifndef RENDERING_PLATFORM
	#define RENDERING_PLATFORM
			enum class ENGINE_TYPE
			{
				GL_ENGINE,
				DX_ENGINE,
				TWO_D_ENGINE
			};
			extern ENGINE_TYPE currentRenderingEngine = ENGINE_TYPE::DX_ENGINE;
	#endif
#endif

#ifdef VX_2D
#ifndef RENDERING_PLATFORM
#define RENDERING_PLATFORM
			enum class ENGINE_TYPE
			{
				GL_ENGINE,
				DX_ENGINE,
				TWO_D_ENGINE
			};
			static ENGINE_TYPE currentRenderingEngine = ENGINE_TYPE::TWO_D_ENGINE;
#endif
#endif

