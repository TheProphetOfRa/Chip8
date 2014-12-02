//
//  Node.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Node.h"

namespace OpenGL
{
    Node* Node::Create()
    {
        auto result = new Node();
        if (result && result->Init())
        {
            return result;
        }
        delete result;
        result = nullptr;
        return nullptr;
    }
    
    bool Node::Init()
    {
        //vertex list
        float points[] =
        {
            -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f
        };
        
        float uv[] =
        {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f
        };
        
        float data[64*32][4];
        for (int i = 0 ; i < 64*32 ; ++i)
        {
            for (int j = 0 ; j < 4 ; ++j)
            {
                data[i][j] = (GLubyte)0.0f;
            }
        }
        
        //vertex bufer object
        GLuint vertexvbo = 0;
        glGenBuffers(1, &vertexvbo); //Generate a vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, vertexvbo); //bind it to the array buffer
        glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW); // fill the buffer with the vertex list
        
        //colours
        GLuint colourvbo = 0;
        glGenBuffers(1, &colourvbo);
        glBindBuffer(GL_ARRAY_BUFFER, colourvbo);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), uv, GL_STATIC_DRAW);
        
        _tex = 0;
        glGenTextures(1, &_tex);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST);
        
        //vertex attribute object
        _vao = 0;
        glGenVertexArrays(1, &_vao); //generate a vao
        glBindVertexArray(_vao); //bind the vao
        glEnableVertexAttribArray(0); //enable attirbute 0 of vao
        glBindBuffer(GL_ARRAY_BUFFER, vertexvbo); //bind the vbo to the array buffer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //fill attribute 0 with vertices made of 3 GL_FLOAT
        glEnableVertexAttribArray(1); // enable attribute 1 of vao
        glBindBuffer(GL_ARRAY_BUFFER, colourvbo); //bind colour vbo to buffer
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr); //fill attribute 1 with vertices made of 3 GL_FLOAT
        
        _shader = OpenGLController::sharedController()->LoadShader("Shaders/array");
        
        return true;
    }
    
    void Node::Draw() const
    {
        glUseProgram (_shader);
        glBindVertexArray (_vao);
        glDrawArrays (GL_TRIANGLES, 0, 6);
    }
    
    void Node::Update()
    {
        
    }
    
    void Node::DrawTexture(const float *data)
    {
        float drawData[64*32][4];
        for (int i = 0 ; i < 64*32 ; ++i)
        {
            for (int j = 0 ; j < 4 ; ++j)
            {
                drawData[i][j] = (GLubyte)data[i];
            }
        }
        
        glBindTexture(GL_TEXTURE_2D, _tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
}