//
//  OpenGLController.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __OpenGL_RTT__OpenGLController__
#define __OpenGL_RTT__OpenGLController__

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

namespace OpenGL
{
    class Node;
    
    class OpenGLController
    {
    public:
        static OpenGLController* sharedController();
        
        void AddNode(Node* n) { _nodes.push_back(n); }
        
        GLuint LoadShader(const std::string& filename) const;
        
        bool Update(std::vector<bool> &keys);
        void Draw();
        
        GLFWwindow* getWindow() const { return _window; }
        
        void Destroy();
    private:
        OpenGLController() {}
        bool Init();
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
        
        bool _windowed = true;
        
        std::vector<Node*> _nodes;
    };
}
#endif /* defined(__OpenGL_RTT__OpenGLController__) */
