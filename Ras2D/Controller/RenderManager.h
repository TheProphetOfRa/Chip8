//
//  RenderManager.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 12/11/2019.
//  Copyright © 2019 David Hodgkinson. All rights reserved.
//

#pragma once

#include "Math/BaseMath.h"

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <string>

namespace Ras2D
{
    class Node;

    class RenderManager
    {
    public:
        bool Init();
        void Destroy();
        
        void SetRootNode(Node* rootNode);
        
        GLuint LoadShader(const std::string& filename) const;

        GLFWwindow* GetWindow() const { return _window; }
        const Size& GetWindowSize() const { return _windowSize; }
        
        bool PreRender() { return true; } //Don't need this yet
        bool Render();
        bool PostRender() { return true; } //Don't need this yet
    private:
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
        
        Node* _rootNode;
    };
}
