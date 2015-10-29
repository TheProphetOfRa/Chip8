//
//  Director.h
//  Ras2D
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __Ras2D__Director__
#define __Ras2D__Director__

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "Math/BaseMath.h"

namespace Ras2D
{
	class Node;

	class Director
	{
	public:
		static Director* GetInstance();

		//TODO offload this to a scene class
		void AddNode(Node* n) { _nodes.push_back(n); }

		//TODO offload to shader manager
		GLuint LoadShader(const std::string& filename) const;

		//This will set up the renderer and shaders and a scheduler
		bool Init();

		bool Update(std::vector<bool> &keys);
		void Draw();

		GLFWwindow* GetWindow() const { return _window; }
		const Size& GetWindowSize() const { return _windowSize; }

		void Destroy();
	private:
		Director() {}
		std::string LoadShaderFromFile(const std::string& fileName) const;
		bool CheckForShaderError(const GLuint shader) const;
		bool CheckForShaderLinkErrors(const GLuint shaderProgram) const;

		bool IsShaderProgramValid(const GLuint shader) const;
	private:
		GLFWmonitor* _monitor;
		const GLFWvidmode* _vmode;
		GLFWwindow* _window;
		const GLubyte* _renderer;
		const GLubyte* _version;

		Size _windowSize;

		bool _windowed = true;

		std::vector<Node*> _nodes;
	};
}
#endif
