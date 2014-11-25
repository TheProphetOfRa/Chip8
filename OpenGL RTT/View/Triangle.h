//
//  Triangle.h
//  OpenGL RTT
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __OpenGL_RTT__Triangle__
#define __OpenGL_RTT__Triangle__

#include "OpenGLController.h"

namespace OpenGL
{
    class Triangle
    {
    public:
        static Triangle* Create();
        
        const GLuint& GetVAO() const { return _vao; }
        const GLuint& GetVBO() const { return _vbo; }
        const GLuint& GetShaderProgram() const { return _shader; }
    private:
        Triangle() {}
        bool Init();
    private:
        GLuint _vao;
        GLuint _vbo;
        GLuint _shader;
    };
}

#endif /* defined(__OpenGL_RTT__Triangle__) */
