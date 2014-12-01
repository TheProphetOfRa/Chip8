//
//  Node.h
//  OpenGL RTT
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __OpenGL_RTT__Node__
#define __OpenGL_RTT__Node__

#include "OpenGLController.h"

namespace OpenGL
{
    class Node
    {
    public:
        static Node* Create();
        
        const GLuint& GetVAO() const { return _vao; }
        const GLuint& GetShaderProgram() const { return _shader; }
        
        virtual void Draw() const;
        virtual void Update();
        
        virtual void DrawTexture(const float * data);
    protected:
        Node() {}
        bool Init();
    protected:
        GLuint _vao;
        GLuint _shader;
        GLuint _tex;
    };
}

#endif /* defined(__OpenGL_RTT__Node__) */
