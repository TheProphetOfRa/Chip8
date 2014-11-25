//
//  OpenGLController.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "OpenGLController.h"

#include "Triangle.h"

#include <assert.h>
#include <fstream>
#include <stdio.h>
#include <streambuf>

namespace OpenGL
{
    static OpenGLController* sharedInstance = nullptr;
    
    OpenGLController* OpenGLController::sharedController()
    {
        if (sharedInstance == nullptr)
        {
            sharedInstance = new OpenGLController();
            if (sharedInstance && sharedInstance->Init())
            {
                return sharedInstance;
            }
            delete sharedInstance;
            sharedInstance = nullptr;
            exit(-1);
        }
        else
        {
            return sharedInstance;
        }
    }
    
    bool OpenGLController::Init()
    {
        //start GL Context and OS Window using glfw
        if (!glfwInit())
        {
            fprintf(stderr, "Error: Could not start GLFW3\n");
            return false;
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        _window = glfwCreateWindow(640, 480, "OpenGL Application", nullptr, nullptr);
        if (!_window)
        {
            fprintf(stderr, "Error: Could not open window with GLFW3\n");
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(_window);
        
        //start GLEW extension handler
        glewExperimental = GL_TRUE;
        glewInit();
        
        _renderer = glGetString(GL_RENDERER);
        _version = glGetString(GL_VERSION);
        
        printf("Renderer: %s\n", _renderer);
        printf("Version: %s\n", _version);
        
        //Tell GL to only draw to a pixel if the shape is closer to the viewer
        glEnable(GL_DEPTH_TEST); //Enable depth testing
        glDepthFunc(GL_LESS); //Interp. smaller values as closer
        
        return true;
    }
    
    GLuint OpenGLController::LoadShader(const std::string &fileName) const
    {
        GLuint shader = glCreateProgram();
        
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        std::string vfilename = fileName + "_vert.glsl";
        std::ifstream vt(vfilename.c_str());
        std::string vstr((std::istreambuf_iterator<char>(vt)),
                   std::istreambuf_iterator<char>());
        const GLchar* vert_shader = vstr.c_str();
        printf("Vertex Shader: %s\n", vstr.c_str());
        glShaderSource(vs, 1, &vert_shader, nullptr);
        glCompileShader(vs);
        
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        std::string ffilename = fileName + "_frag.glsl";
        std::ifstream ft(vfilename.c_str());
        std::string fstr((std::istreambuf_iterator<char>(ft)),
                   std::istreambuf_iterator<char>());
        const GLchar* frag_shader = fstr.c_str();
        printf("Fragment Shader: %s\n", fstr.c_str());
        glShaderSource(fs, 1, &frag_shader, nullptr);
        glCompileShader(fs);
        
        glAttachShader(shader, fs);
        glAttachShader(shader, vs);
        glLinkProgram(shader);
        
        return shader;
    }
    
    bool OpenGLController::update()
    {

        return !glfwWindowShouldClose(_window);
    }
    
    void OpenGLController::draw() const
    {
        glClearColor(0.6f, 0.6f, 0.6f, 0.8f);
        
        for (const auto t : _triangles)
        {
            //clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glUseProgram(t->GetShaderProgram());
            glBindVertexArray(t->GetVAO());
            
            glDrawArrays(GL_TRIANGLES, 0, 3);
            // update other events like input handling
            glfwPollEvents();
            //draw buffers to the display
            glfwSwapBuffers(_window);
        }
    }
    
    void OpenGLController::Destroy()
    {
        glfwTerminate();
    }
}