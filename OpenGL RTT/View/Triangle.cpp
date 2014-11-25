//
//  Triangle.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Triangle.h"

namespace OpenGL
{
    Triangle* Triangle::Create()
    {
        auto result = new Triangle();
        if (result && result->Init())
        {
            return result;
        }
        delete result;
        result = nullptr;
        return nullptr;
    }
    
    bool Triangle::Init()
    {
        float points[] =
        {
            0.0f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };
        
        _vbo = 0;
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
        
        _vao = 0;
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        _shader = OpenGLController::sharedController()->LoadShader("triangle");

        return true;
    }
}