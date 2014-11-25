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
        //vertex list
        float points[] =
        {
            0.0f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };
        
        //vertex bufer object
        _vbo = 0;
        glGenBuffers(1, &_vbo); //Generate a vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, _vbo); //bind it to the array buffer
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW); // fill the buffer with the vertex list
        
        //vertex attribute object
        _vao = 0;
        glGenVertexArrays(1, &_vao); //generate a vao
        glBindVertexArray(_vao); //bind the vao to the arrray buffer
        glEnableVertexAttribArray(0); //do a thing?
        glBindBuffer(GL_ARRAY_BUFFER, _vbo); //bind the vbo to the array buffer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //
        
        _shader = OpenGLController::sharedController()->LoadShader("triangle");

        return true;
    }
}