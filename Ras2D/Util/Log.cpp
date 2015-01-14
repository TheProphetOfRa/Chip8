//
//  Log.cpp
//  Ras2D
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Log.h"

#include "FileUtils.h"

namespace Ras2D
{
	static const char * kLogFile = "Ras2D.log";

	const char* GL_type_to_string(GLenum type) {
		switch (type) {
		case GL_BOOL: return "bool";
		case GL_INT: return "int";
		case GL_FLOAT: return "float";
		case GL_FLOAT_VEC2: return "vec2";
		case GL_FLOAT_VEC3: return "vec3";
		case GL_FLOAT_VEC4: return "vec4";
		case GL_FLOAT_MAT2: return "mat2";
		case GL_FLOAT_MAT3: return "mat3";
		case GL_FLOAT_MAT4: return "mat4";
		case GL_SAMPLER_2D: return "sampler2D";
		case GL_SAMPLER_3D: return "sampler3D";
		case GL_SAMPLER_CUBE: return "samplerCube";
		case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		default: break;
		}
		return "other";
	}

	bool Log::Restart()
	{
		time_t now = time(nullptr);
		char* date = nullptr;

		//date = ctime(&now);
		printf("GL_LOG_FILE log. local time %s\n", std::to_string(now).c_str());

		return true;
	}

	void Log::LogGLParams()
	{
		GLenum params[] = {
			GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			GL_MAX_DRAW_BUFFERS,
			GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
			GL_MAX_TEXTURE_IMAGE_UNITS,
			GL_MAX_TEXTURE_SIZE,
			GL_MAX_VARYING_FLOATS,
			GL_MAX_VERTEX_ATTRIBS,
			GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
			GL_MAX_VERTEX_UNIFORM_COMPONENTS,
			GL_MAX_VIEWPORT_DIMS,
			GL_STEREO,
		};
		const char* names[] = {
			"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
			"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
			"GL_MAX_DRAW_BUFFERS",
			"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
			"GL_MAX_TEXTURE_IMAGE_UNITS",
			"GL_MAX_TEXTURE_SIZE",
			"GL_MAX_VARYING_FLOATS",
			"GL_MAX_VERTEX_ATTRIBS",
			"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
			"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
			"GL_MAX_VIEWPORT_DIMS",
			"GL_STEREO",
		};
		Log("GL Context Params:\n");
		// integers - only works if the order is 0-10 integer return types
		for (int i = 0; i < 10; i++) {
			int v = 0;
			glGetIntegerv(params[i], &v);
			Log("%s %i\n", names[i], v);
		}
		// others
		int v[2];
		v[0] = v[1] = 0;
		glGetIntegerv(params[10], v);
		Log("%s %i %i\n", names[10], v[0], v[1]);
		unsigned char s = 0;
		glGetBooleanv(params[11], &s);
		Log("%s %u\n", names[11], (unsigned int)s);
		Log("-----------------------------\n");
	}

	bool Log::Log(const char *message, ...)
	{
		va_list argptr;

		va_start(argptr, message);
		vprintf(message, argptr);
		va_end(argptr);

		return true;
	}

	bool Log::Error(const char* message, ...)
	{
		va_list argptr;

		va_start(argptr, message);
		vprintf(message, argptr);
		va_end(argptr);

		va_start(argptr, message);
		vfprintf(stderr, message, argptr);
		va_end(argptr);

		return true;
	}

	void Log::LogShader(GLuint shaderProgram)
	{
		Log("--------------------\nshader programme %i info:\n", shaderProgram);
		int params = -1;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &params);
		Log("GL_LINK_STATUS = %i\n", params);

		glGetProgramiv(shaderProgram, GL_ATTACHED_SHADERS, &params);
		Log("GL_ATTACHED_SHADERS = %i\n", params);

		glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &params);
		Log("GL_ACTIVE_ATTRIBUTES = %i\n", params);

		for (int i = 0; i < params; i++)
		{
			char name[64];
			int max_length = 64;
			int actual_length = 0;
			int size = 0;
			GLenum type;
			glGetActiveAttrib(shaderProgram, i, max_length, &actual_length, &size, &type, name);
			if (size > 1)
			{
				for (int j = 0; j < size; j++)
				{
					char long_name[64];
					Log(long_name, "%s[%i]", name, j);
					int location = glGetAttribLocation(shaderProgram, long_name);
					Log("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), long_name, location);
				}
			}
			else
			{
				int location = glGetAttribLocation(shaderProgram, name);
				Log("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), name, location);
			}
		}

		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &params);
		Log("GL_ACTIVE_UNIFORMS = %i\n", params);
		for (int i = 0; i < params; i++)
		{
			char name[64];
			int max_length = 64;
			int actual_length = 0;
			int size = 0;
			GLenum type;
			glGetActiveUniform(shaderProgram, i, max_length, &actual_length, &size, &type, name);
			if (size > 1)
			{
				for (int j = 0; j < size; j++)
				{
					char long_name[64];
					Log(long_name, "%s[%i]", name, j);
					int location = glGetUniformLocation(shaderProgram, long_name);
					Log("  %i) type:%s name:%s location:%i\n",
						i, GL_type_to_string(type), long_name, location);
				}
			}
			else
			{
				int location = glGetUniformLocation(shaderProgram, name);
				Log("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), name, location);
			}
		}

		int max_length = 2048;
		int actual_length = 0;
		char log[2048];
		glGetProgramInfoLog(shaderProgram, max_length, &actual_length, log);
		Log("program info log for GL index %u:\n%s", shaderProgram, log);
	}
}