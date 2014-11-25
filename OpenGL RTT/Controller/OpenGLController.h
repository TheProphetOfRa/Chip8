//
//  OpenGLController.h
//  OpenGL RTT
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
    class Triangle;
    
    class OpenGLController
    {
    public:
        static OpenGLController* sharedController();
        
        void addTriangle(const Triangle* t) { _triangles.push_back(t); }
        
        GLuint LoadShader(const std::string& fileName) const;
        
        bool update();
        void draw() const;
        
        GLFWwindow* getWindow() const { return  _window; }
        
        void Destroy();
    private:
        OpenGLController() {}
        bool Init();
    private:
        GLFWwindow* _window;
        const GLubyte* _renderer;
        const GLubyte* _version;
        
        std::vector<const Triangle*> _triangles;
    };
}
#endif /* defined(__OpenGL_RTT__OpenGLController__) */
